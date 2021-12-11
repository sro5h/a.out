#include "server.h"

#include <common/log.h>

#include <enet/enet.h>
#include <string.h>

static void aout_server_on_connect(
                aout_server* server,
                ENetPeer* peer);

static void aout_server_on_receive(
                aout_server* server,
                ENetPeer* peer,
                ENetPacket* packet);

static void aout_server_on_disconnect(
                aout_server* server,
                ENetPeer* peer);

static aout_res aout_server_create_packet(
                aout_sv_msg_type type,
                size_t size,
                ENetPacket** packet,
                aout_stream* stream);

static uint32_t aout_server_get_packet_flags(
                aout_sv_msg_type type);

aout_server* aout_server_create(
                aout_server_adapter adapter) {
        // Connections must be zero
        aout_server* server = calloc(1, sizeof(*server));

        if (!server) {
                return NULL;
        }

        server->adapter = adapter;

        server->host = enet_host_create(
                &(ENetAddress) {
                        .host = ENET_HOST_ANY,
                        .port = 42424,
                },
                AOUT_SERVER_MAX_CONNECTIONS,
                2,
                0,
                0
        );

        if (!server->host) {
                // TODO: Could be changed to mirror application error handling
                free(server);
                return NULL;
        }

        return server;
}

void aout_server_destroy(
                aout_server* server) {
        if (server) {
                enet_host_destroy(server->host);
                free(server);
        }
}

void aout_server_update(
                aout_server* server) {
        assert(server);

        ENetEvent event;
        while (enet_host_service(server->host, &event, 0) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                        aout_server_on_connect(server, event.peer);
                        break;
                case ENET_EVENT_TYPE_RECEIVE:
                        aout_server_on_receive(server, event.peer, event.packet);
                        enet_packet_destroy(event.packet);
                        break;
                case ENET_EVENT_TYPE_DISCONNECT:
                        aout_server_on_disconnect(server, event.peer);
                        break;
                case ENET_EVENT_TYPE_NONE:
                        aout_loge("invalid event type");
                        break;
                }
        }

        // Send all queued packets.
        enet_host_flush(server->host);
}

aout_res aout_server_send_msg_connection(
                aout_server* server,
                uint16_t peer_id,
                aout_sv_msg_connection* msg) {
        assert(server); assert(msg);
        assert(server->host);
        assert(peer_id < server->host->peerCount);
        assert(server->host->peers[peer_id].connectID); // TODO: Treat as error?

        // The requested buffer is an upper limit of how much space will be
        // needed and will be shrunken.
        ENetPacket* packet;
        aout_stream stream;
        aout_res res = aout_server_create_packet(
                AOUT_SV_MSG_TYPE_CONNECTION,
                sizeof(*msg),
                &packet,
                &stream
        );

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write sv_msg_connection header");
                goto error;
        }

        res = aout_stream_write_sv_msg_connection(&stream, msg);

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write sv_msg_connection");
                goto error;
        }

        // Resize the packet to the number of written bytes. Allows optimisation
        // of aout_stream_{write,read}_sv_msg_connection functions.
        // Doesn't involve reallocations (cheap)
        enet_packet_resize(packet, aout_stream_get_count(&stream));

        ENetPeer* peer = &server->host->peers[peer_id];
        // Ownership of packet is transferred, if enet_peer_send succeeds!
        if (enet_peer_send(peer, 0, packet) < 0) {
                aout_loge("could not send packet");
                goto error;
        }

        return AOUT_OK;

error:
        assert(packet); assert(packet->referenceCount == 0);
        if (packet->referenceCount == 0) { // TODO: Shouldn't be necessary
                enet_packet_destroy(packet);
        }

        return AOUT_ERR(AOUT_SERVER_ERR);
}

aout_res aout_server_send_msg_state(
                aout_server* server,
                uint16_t peer_id,
                aout_sv_msg_state* msg) {
        assert(server); assert(msg);
        assert(server->host);
        assert(peer_id < server->host->peerCount);
        assert(server->host->peers[peer_id].connectID); // TODO: Treat as error?

        // The requested buffer is an upper limit of how much space will be
        // needed and will be shrunken.
        ENetPacket* packet;
        aout_stream stream;
        aout_res res = aout_server_create_packet(
                AOUT_SV_MSG_TYPE_STATE,
                sizeof(*msg),
                &packet,
                &stream
        );

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write sv_msg_state header");
                goto error;
        }

        res = aout_stream_write_sv_msg_state(&stream, msg);

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write sv_msg_state");
                goto error;
        }

        // Resize the packet to the number of written bytes
        enet_packet_resize(packet, aout_stream_get_count(&stream));

        ENetPeer* peer = &server->host->peers[peer_id];
        if (enet_peer_send(peer, 0, packet) < 0) {
                aout_loge("could not send packet");
                goto error;
        }

        return AOUT_OK;

error:
        assert(packet); assert(packet->referenceCount == 0);

        enet_packet_destroy(packet);
        return AOUT_ERR(AOUT_SERVER_ERR);
}

static void aout_server_on_connect(
                aout_server* server,
                ENetPeer* peer) {
        assert(server); assert(peer);

        uint16_t peer_id = peer->incomingPeerID;
        aout_connection* connection = &server->connections[peer_id];
        assert(connection->id == 0);

        connection->id = peer->connectID;
        connection->peer_id = peer_id;
        peer->data = connection;

        aout_logd("[0x%08x] connection", connection->id);

        // Should be sent to all the other connected peers!
        aout_res res = aout_server_send_msg_connection(
                server, connection->peer_id, &(aout_sv_msg_connection) {
                        .id = connection->id,
                        .peer_id = connection->peer_id
                }
        );
        assert(AOUT_IS_OK(res)); // TODO: Maybe print error message

        aout_server_adapter* adapter = &server->adapter;
        if (adapter->on_connection) {
                adapter->on_connection(server, *connection, adapter->context);
        }
}

static void aout_server_on_receive(
                aout_server* server,
                ENetPeer* peer,
                ENetPacket* packet) {
        assert(server); assert(peer); assert(packet);
        assert(packet->data);

        aout_connection const* connection = peer->data;
        assert(connection->id == peer->connectID);
}

static void aout_server_on_disconnect(
                aout_server* server,
                ENetPeer* peer) {
        assert(server); assert(peer);
        assert(peer->data);

        aout_connection* connection = peer->data;
        assert(connection->id != 0);

        aout_logd("[0x%08x] disconnection", connection->id);

        aout_server_adapter* adapter = &server->adapter;
        if (adapter->on_disconnection) {
                adapter->on_disconnection(server, *connection, adapter->context);
        }

        *connection = (aout_connection) { 0 };
        peer->data = NULL;
}

static aout_res aout_server_create_packet(
                aout_sv_msg_type type,
                size_t size,
                ENetPacket** packet,
                aout_stream* stream) {
        assert(packet); assert(stream);

        *packet = enet_packet_create(
                NULL,
                sizeof(AOUT_TYPE_SV_MSG_TYPE) + size,
                aout_server_get_packet_flags(type)
        );

        assert(*packet);

        *stream = (aout_stream) { // Make sure index is initialised
                .data = (*packet)->data,
                .data_size = (*packet)->dataLength
        };

        return aout_stream_write_sv_msg_type(stream, type);
}

static uint32_t aout_server_get_packet_flags(
                aout_sv_msg_type type) {
        switch (type) {
        case AOUT_SV_MSG_TYPE_CONNECTION:
                return ENET_PACKET_FLAG_RELIABLE;
        default:
                return ENET_PACKET_FLAG_RELIABLE; // TODO: Change?
        }
}
