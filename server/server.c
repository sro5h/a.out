#include "server.h"

#include <enet/enet.h>
#include <stdio.h>
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
                void) {
        aout_server* server = malloc(sizeof(*server));

        if (!server) {
                return NULL;
        }

        server->host = enet_host_create(
                &(ENetAddress) {
                        .host = ENET_HOST_ANY,
                        .port = 42424,
                },
                MAX_CONNECTIONS,
                2,
                0,
                0
        );

        if (!server->host) {
                free(server);
                return NULL;
        }

        memset(server->connections, 0, sizeof(server->connections));
        server->is_running = true;

        return server;
}

void aout_server_destroy(
                aout_server* server) {
        enet_host_destroy(server->host);
        free(server);
}

void aout_server_update(
                aout_server* server) {
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
                        //fprintf(stderr, "Invalid event\n");
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
                printf("error: could not write sv_msg_type\n");
                goto error;
        }

        res = aout_stream_write_sv_msg_connection(&stream, msg);

        if (AOUT_IS_ERR(res)) {
                printf("error: could not write sv_msg_connection\n");
                goto error;
        }

        // Resize the packet to the number of written bytes. Allows optimisation
        // of aout_stream_{write,read}_sv_msg_connection functions.
        // Doesn't involve reallocations (cheap)
        enet_packet_resize(packet, aout_stream_get_count(&stream));

        ENetPeer* peer = &server->host->peers[peer_id];
        // Ownership is transferred, if enet_peer_send succeeds!
        if (enet_peer_send(peer, 0, packet) < 0) {
                printf("error: could not send packet\n");
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

bool aout_server_is_running(
                aout_server* server) {
        return server->is_running;
}

static void aout_server_on_connect(
                aout_server* server,
                ENetPeer* peer) {
        aout_connection* connection = &server->connections[peer->outgoingPeerID];
        connection->id = peer->connectID;
        connection->peer_id = peer->outgoingPeerID;
        peer->data = connection;

        printf("connection from %u\n", connection->id);

        // Should be sent to all the other connected peers!
        aout_res res = aout_server_send_msg_connection(
                server, connection->peer_id, &(aout_sv_msg_connection) {
                        .id = connection->id,
                        .peer_id = connection->peer_id
                }
        );
        assert(AOUT_IS_OK(res)); // TODO: Maybe print error message
}

static void aout_server_on_receive(
                aout_server* server,
                ENetPeer* peer,
                ENetPacket* packet) {
        (void) server;
        (void) peer;
        (void) packet;
}

static void aout_server_on_disconnect(
                aout_server* server,
                ENetPeer* peer) {
        (void) server;

        aout_connection* connection = (aout_connection*) peer->data;
        printf("disconnection from %u\n", connection->id);

        peer->data = NULL;
}

static aout_res aout_server_create_packet(
                aout_sv_msg_type type,
                size_t size,
                ENetPacket** packet,
                aout_stream* stream) {
        assert(packet);

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
