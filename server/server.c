#include "server.h"

#include <common/host.h>
#include <common/log.h>

#include <enet/enet.h>
#include <string.h>

static void aout_server_on_connect(
                aout_server* self,
                ENetPeer* peer);

static void aout_server_on_disconnect(
                aout_server* self,
                ENetPeer* peer);

static void aout_server_on_receive(
                aout_server* self,
                ENetPeer* peer,
                ENetPacket* packet);

static void aout_server_on_receive_msg_input(
                aout_server* self,
                aout_connection const* connection,
                aout_stream* stream);

static aout_res aout_wrapper_write_sv_msg_connection(
                aout_stream* self,
                void* msg);

static aout_res aout_wrapper_write_sv_msg_state(
                aout_stream* self,
                void* msg);

aout_server* aout_server_create(
                aout_server_adapter adapter,
                size_t connection_count) {
        // Connections must be zero
        aout_server* self = calloc(1, sizeof(*self));

        if (!self) {
                return NULL;
        }

        self->adapter = adapter;

        size_t const connection_size = sizeof(*self->connections);
        self->connections = calloc(connection_count, connection_size);

        if (!self->connections) {
                goto error;
        }

        self->host = enet_host_create(
                &(ENetAddress) {
                        .host = ENET_HOST_ANY,
                        .port = 42424,
                },
                connection_count,
                2,
                0,
                0
        );

        if (!self->host) {
                goto error;
        }

        return self;

error:
        aout_server_destroy(self);
        return NULL;
}

void aout_server_destroy(
                aout_server* self) {
        if (self) {
                enet_host_destroy(self->host);
                free(self->connections);
                free(self);
        }
}

void aout_server_update(
                aout_server* self) {
        assert(self);

        ENetEvent event;
        while (enet_host_service(self->host, &event, 0) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                        aout_server_on_connect(self, event.peer);
                        break;
                case ENET_EVENT_TYPE_RECEIVE:
                        aout_server_on_receive(self, event.peer, event.packet);
                        enet_packet_destroy(event.packet);
                        break;
                case ENET_EVENT_TYPE_DISCONNECT:
                        aout_server_on_disconnect(self, event.peer);
                        break;
                case ENET_EVENT_TYPE_NONE:
                        aout_loge("invalid event type");
                        break;
                }
        }

        // Send all queued packets.
        enet_host_flush(self->host);
}

void aout_server_flush(
                aout_server* self) {
        assert(self);
        enet_host_flush(self->host);
}

aout_res aout_server_send_msg_connection(
                aout_server* self,
                uint16_t peer_id,
                aout_sv_msg_connection* msg) {
        assert(self); assert(msg);
        assert(self->host);
        assert(peer_id < self->host->peerCount);
        assert(self->host->peers[peer_id].connectID); // TODO: Treat as error?

        return aout_host_send_msg(self->host, peer_id, &(aout_msg_desc) {
                .type = AOUT_SV_MSG_TYPE_CONNECTION,
                .size = sizeof(*msg),
                .flags = ENET_PACKET_FLAG_RELIABLE,
                .value = msg,
                .write = aout_wrapper_write_sv_msg_connection,
        });
}

aout_res aout_server_send_msg_state(
                aout_server* self,
                uint16_t peer_id,
                aout_sv_msg_state* msg) {
        assert(self); assert(msg);
        assert(self->host);
        assert(peer_id < self->host->peerCount);
        assert(self->host->peers[peer_id].connectID); // TODO: Treat as error?

        return aout_host_send_msg(self->host, peer_id, &(aout_msg_desc) {
                .type = AOUT_SV_MSG_TYPE_STATE,
                .size = sizeof(*msg),
                .flags = ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
                       | ENET_PACKET_FLAG_UNSEQUENCED,
               .value = msg,
               .write = aout_wrapper_write_sv_msg_state,
        });
}

void aout_server_on_connect(
                aout_server* self,
                ENetPeer* peer) {
        assert(self); assert(peer);

        uint16_t peer_id = peer->incomingPeerID;
        aout_connection* connection = &self->connections[peer_id];
        assert(connection->id == 0);

        connection->id = peer->connectID;
        connection->peer_id = peer_id;
        peer->data = connection;

        aout_logd("[0x%08x] connection", connection->id);

        // Should be sent to all the other connected peers!
        aout_res res = aout_server_send_msg_connection(
                self, connection->peer_id, &(aout_sv_msg_connection) {
                        .id = connection->id,
                        .peer_id = connection->peer_id
                }
        );
        assert(AOUT_IS_OK(res)); // TODO: Maybe print error message

        aout_server_adapter* adapter = &self->adapter;
        if (adapter->on_connection) {
                adapter->on_connection(self, *connection, adapter->context);
        }
}

void aout_server_on_disconnect(
                aout_server* self,
                ENetPeer* peer) {
        assert(self); assert(peer);
        assert(peer->data);

        aout_connection* connection = peer->data;
        assert(connection->id != 0);

        aout_logd("[0x%08x] disconnection", connection->id);

        aout_server_adapter* adapter = &self->adapter;
        if (adapter->on_disconnection) {
                adapter->on_disconnection(self, *connection, adapter->context);
        }

        *connection = (aout_connection) { 0 };
        peer->data = NULL;
}

void aout_server_on_receive(
                aout_server* self,
                ENetPeer* peer,
                ENetPacket* packet) {
        assert(self); assert(peer); assert(packet);
        assert(packet->data);

        aout_connection const* connection = peer->data;
        assert(connection->id == peer->connectID);

        aout_stream stream = {
                .data = packet->data,
                .data_size = packet->dataLength
        };

        aout_msg_type type;
        static_assert(sizeof(aout_msg_type) == sizeof(uint16_t));

        if (AOUT_IS_ERR(aout_stream_read_u16(&stream, &type))) {
                aout_loge("could not read cl_msg_type");
                return;
        }

        switch (type) {
        case AOUT_CL_MSG_TYPE_INPUT:
                aout_server_on_receive_msg_input(self, connection, &stream);
                break;
        default:
                aout_loge("unknown cl_msg_type");
                break;
        }
}

void aout_server_on_receive_msg_input(
                aout_server* self,
                aout_connection const* connection,
                aout_stream* stream) {
        assert(self); assert(connection); assert(stream);

        aout_cl_msg_input msg;
        if (AOUT_IS_ERR(aout_stream_read_cl_msg_input(stream, &msg))) {
                aout_loge("could not read cl_msg_input");
                return;
        }

        if (self->adapter.on_msg_input) {
                self->adapter.on_msg_input(
                        self, *connection, &msg, self->adapter.context
                );
        }
}

aout_res aout_wrapper_write_sv_msg_connection(
                aout_stream* self,
                void* msg) {
        return aout_stream_write_sv_msg_connection(self, msg);
}

aout_res aout_wrapper_write_sv_msg_state(
                aout_stream* self,
                void* msg) {
        return aout_stream_write_sv_msg_state(self, msg);
}
