#include "client.h"

#include <common/byte_order.h>
#include <common/log.h>
#include <common/messages.h>

#include <enet/enet.h>

static void aout_client_on_connect(
                aout_client* self,
                ENetPeer* peer);

static void aout_client_on_disconnect(
                aout_client* self,
                ENetPeer* peer);

static void aout_client_on_receive(
                aout_client* self,
                ENetPeer* peer, // TODO: Maybe use peer_id or aout_connection
                ENetPacket* packet);

static void aout_client_on_receive_msg_connection(
                aout_client* self,
                aout_stream* stream);

static void aout_client_on_receive_msg_state(
                aout_client* self,
                aout_stream* stream);

static aout_res aout_client_create_packet(
                aout_cl_msg_type type,
                size_t size,
                ENetPacket** packet,
                aout_stream* stream);

static uint32_t aout_client_get_packet_flags(
                aout_cl_msg_type type);

aout_client* aout_client_create(
                aout_client_adapter adapter) {
        aout_client* self = calloc(1, sizeof(*self));

        if (!self) {
                return NULL;
        }

        self->connection = (aout_connection) { 0 };
        self->adapter = adapter;

        self->host = enet_host_create(NULL, 1, 2, 0, 0);

        if (!self->host) {
                // TODO: Could be changed to mirror application error handling
                free(self);
                return NULL;
        }

        return self;
}

void aout_client_destroy(
                aout_client* self) {
        if (self) {
                enet_host_destroy(self->host);
                free(self);
        }
}

void aout_client_update(
                aout_client* self) {
        assert(self);

        ENetEvent event;
        while (enet_host_service(self->host, &event, 0) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                        aout_client_on_connect(self, event.peer);
                        break;
                case ENET_EVENT_TYPE_RECEIVE:
                        aout_client_on_receive(self, event.peer, event.packet);
                        enet_packet_destroy(event.packet);
                        break;
                case ENET_EVENT_TYPE_DISCONNECT:
                        aout_client_on_disconnect(self, event.peer);
                        break;
                case ENET_EVENT_TYPE_NONE:
                        break;
                }
        }
}

aout_res aout_client_connect(
                aout_client* self,
                uint32_t ip,
                uint16_t port) {
        assert(self);

        ENetAddress address;
        address.host = aout_hton_u32(ip);
        address.port = port;

        ENetPeer* peer = enet_host_connect(self->host, &address, 2, 0);

        if (!peer) {
                return AOUT_ERR(AOUT_CLIENT_ERR);
        }

        return AOUT_OK;
}

aout_res aout_client_send_msg_input(
                aout_client* self,
                aout_cl_msg_input* msg) {
        assert(self); assert(msg);
        assert(self->host);
        assert(self->peer);

        ENetPacket* packet;
        aout_stream stream;
        aout_res res = aout_client_create_packet(
                AOUT_CL_MSG_TYPE_INPUT,
                sizeof(*msg),
                &packet,
                &stream
        );

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write cl_msg_input header");
                goto error;
        }

        res = aout_stream_write_cl_msg_input(&stream, msg);

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write cl_msg_input");
                goto error;
        }

        // Resize the packet to the number of written bytes
        enet_packet_resize(packet, aout_stream_get_count(&stream));

        // Ownership of packet is transferred, if enet_peer_send succeeds!
        if (enet_peer_send(self->peer, 0, packet) < 0) {
                aout_loge("could not send cl_msg_input");
                goto error;
        }

        return AOUT_OK;

error:
        assert(packet); assert(packet->referenceCount == 0);
        enet_packet_destroy(packet);

        return AOUT_ERR(AOUT_CLIENT_ERR);
}

void aout_client_flush(
                aout_client* self) {
        assert(self);
        enet_host_flush(self->host);
}

aout_connection aout_client_get_connection(
                aout_client const* self) {
        assert(self);
        return self->connection;
}

static void aout_client_on_connect(
                aout_client* self,
                ENetPeer* peer) {
        assert(self); assert(peer);
        assert(self->peer == NULL);

        aout_connection* connection = &self->connection;
        assert(connection->id == 0);

        connection->id = peer->connectID;
        connection->peer_id = peer->outgoingPeerID;
        self->peer = peer;

        aout_logd("[0x%08x] connection", connection->id);

        aout_client_adapter* adapter = &self->adapter;
        if (adapter->on_connection) {
                adapter->on_connection(self, adapter->context);
        }
}

static void aout_client_on_disconnect(
                aout_client* self,
                ENetPeer* peer) {
        assert(self); assert(peer);
        assert(self->peer);

        aout_connection* connection = &self->connection;
        assert(connection->id != 0);

        aout_logd("[0x%08x] disconnection", connection->id);

        aout_client_adapter* adapter = &self->adapter;
        if (adapter->on_disconnection) {
                adapter->on_disconnection(self, adapter->context);
        }

        self->connection = (aout_connection) { 0 };
        self->peer = NULL;
}

static void aout_client_on_receive(
                aout_client* self,
                ENetPeer* peer,
                ENetPacket* packet) {
        assert(self); assert(peer); assert(packet);
        assert(self->peer);
        assert(packet->data);

        aout_connection const* connection = &self->connection;
        assert(connection->id == peer->connectID);

        // TODO: Remove below
        aout_logd("[0x%08x] packet received", connection->id);

        aout_stream stream = {
                .data = packet->data,
                .data_size = packet->dataLength
        };

        aout_sv_msg_type type;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_type(&stream, &type))) {
                aout_loge("could not read sv_msg_type");
                return;
        }

        switch (type) {
        case AOUT_SV_MSG_TYPE_CONNECTION:
                aout_client_on_receive_msg_connection(self, &stream);
                break;
        case AOUT_SV_MSG_TYPE_STATE:
                aout_client_on_receive_msg_state(self, &stream);
                break;
        default:
                aout_loge("unknown sv_msg_type");
                break;
        }
}

static void aout_client_on_receive_msg_connection(
                aout_client* self,
                aout_stream* stream) {
        assert(self); assert(stream);

        aout_sv_msg_connection msg;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_connection(stream, &msg))) {
                aout_loge("could not read sv_msg_connection");
                return;
        }

        aout_logd("[0x%08x] sv_msg_connection received: ",
                        self->connection.id);
        aout_logd("{ .id = 0x%x, .peer_id = 0x%x }", msg.id, msg.peer_id);

        aout_client_adapter* adapter = &self->adapter;
        if (adapter->on_msg_connection) {
                adapter->on_msg_connection(self, &msg, adapter->context);
        }
}

static void aout_client_on_receive_msg_state(
                aout_client* self,
                aout_stream* stream) {
        assert(self); assert(stream);

        aout_sv_msg_state msg;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_state(stream, &msg))) {
                aout_loge("could not read sv_msg_state");
                return;
        }

        aout_logd("[0x%08x] sv_msg_state received: ", self->connection.id);
        aout_logd("{ .state.p = { %f, %f } }", msg.state.p.x, msg.state.p.y);

        aout_client_adapter* adapter = &self->adapter;
        if (adapter->on_msg_state) {
                adapter->on_msg_state(self, &msg, adapter->context);
        }
}

static aout_res aout_client_create_packet(
                aout_cl_msg_type type,
                size_t size,
                ENetPacket** packet,
                aout_stream* stream) {
        assert(packet); assert(stream);

        *packet = enet_packet_create(
                NULL,
                sizeof(AOUT_TYPE_CL_MSG_TYPE) + size,
                aout_client_get_packet_flags(type)
        );

        assert(*packet);

        *stream = (aout_stream) {
                .data = (*packet)->data,
                .data_size = (*packet)->dataLength
        };

        return aout_stream_write_cl_msg_type(stream, type);
}

static uint32_t aout_client_get_packet_flags(
                aout_cl_msg_type type) {
        switch (type) {
        case AOUT_CL_MSG_TYPE_INPUT:
                return ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
                        | ENET_PACKET_FLAG_UNSEQUENCED;
        default:
                return ENET_PACKET_FLAG_RELIABLE;
        }
}
