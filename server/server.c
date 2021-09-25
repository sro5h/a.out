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
