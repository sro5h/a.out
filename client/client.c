#include "client.h"

#include <common/byte_order.h>
#include <common/host.h>
#include <common/log.h>
#include <common/memory.h>
#include <common/platform.h>

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

static aout_res aout_wrapper_write_cl_msg_input(
                aout_stream* self,
                void* msg);

void aout_client_ctor(
                aout_client* self,
                aout_client_adapter adapter) {
        assert(self);

        *self = (aout_client) { 0 };

        self->host = enet_host_create(NULL, 1, 2, 0, 0);
        aout_abort_if(!self->host);

        self->adapter = adapter;
}

void aout_client_dtor(
                aout_client* self) {
        assert(self);

        enet_host_destroy(self->host);
        *self = (aout_client) { 0 };
}

aout_client* aout_client_new(
                aout_client_adapter adapter) {
        aout_client* self = aout_acquire(sizeof(*self));
        aout_client_ctor(self, adapter);

        return self;
}

void aout_client_del(
                aout_client** out_self) {
        assert(out_self);

        if (*out_self) {
                aout_client_dtor(*out_self);
                aout_release(*out_self);
                *out_self = NULL;
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

void aout_client_flush(
                aout_client* self) {
        assert(self);
        enet_host_flush(self->host);
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
                return AOUT_ERR;
        }

        return AOUT_OK;
}

aout_res aout_client_send_msg_input(
                aout_client* self,
                aout_cl_msg_input* msg) {
        assert(self); assert(msg);
        assert(self->host);
        assert(self->peer);

        uint16_t peer_id = self->peer->incomingPeerID;
        return aout_host_send_msg(self->host, peer_id, &(aout_msg_desc) {
                .type = AOUT_CL_MSG_TYPE_INPUT,
                .size = sizeof(*msg),
                .flags = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
                       | ENET_PACKET_FLAG_UNSEQUENCED,
                .value = msg,
                .write = aout_wrapper_write_cl_msg_input,
        });
}

aout_connection aout_client_get_connection(
                aout_client const* self) {
        assert(self);
        return self->connection;
}

void aout_client_on_connect(
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

void aout_client_on_disconnect(
                aout_client* self,
                ENetPeer* peer) {
        assert(self); assert(peer);
        assert(self->peer); // TODO: This will be NULL on failed connection

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

void aout_client_on_receive(
                aout_client* self,
                ENetPeer* peer,
                ENetPacket* packet) {
        assert(self); assert(peer); assert(packet);
        assert(self->peer);
        assert(packet->data);

        aout_connection const* connection = &self->connection;
        assert(connection->id == peer->connectID);

        aout_stream stream = {
                .data = packet->data,
                .data_size = packet->dataLength
        };

        aout_msg_type type;
        static_assert(sizeof(aout_msg_type) == sizeof(uint16_t));

        if (AOUT_IS_ERR(aout_stream_read_u16(&stream, &type))) {
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

void aout_client_on_receive_msg_connection(
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

        if (self->adapter.on_msg_connection) {
                self->adapter.on_msg_connection(self, &msg, self->adapter.context);
        }
}

void aout_client_on_receive_msg_state(
                aout_client* self,
                aout_stream* stream) {
        assert(self); assert(stream);

        aout_sv_msg_state msg;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_state(stream, &msg))) {
                aout_loge("could not read sv_msg_state");
                return;
        }

        if (self->adapter.on_msg_state) {
                self->adapter.on_msg_state(self, &msg, self->adapter.context);
        }
}

aout_res aout_wrapper_write_cl_msg_input(
                aout_stream* self,
                void* msg) {
        return aout_stream_write_cl_msg_input(self, msg);
}
