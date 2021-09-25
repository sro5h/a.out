#include "client.h"
#include <enet/enet.h>
#include <stdio.h>

static void aout_client_on_connect(
                aout_client* client,
                ENetPeer* peer);

static void aout_client_on_receive(
                aout_client* client,
                ENetPeer* peer,
                ENetPacket* packet);

static void aout_client_on_disconnect(
                aout_client* client,
                ENetPeer* peer);

aout_client* aout_client_create(
                void) {
        aout_client* client = malloc(sizeof(*client));

        if (!client) {
                return NULL;
        }

        client->host = enet_host_create(NULL, 1, 2, 0, 0);

        if (!client->host) {
                free(client);
                return NULL;
        }

        client->connection = (aout_connection) { 0 };
        client->is_running = true;

        return client;
}

void aout_client_destroy(
                aout_client* client) {
        enet_host_destroy(client->host);
        free(client);
}

void aout_client_update(
                aout_client* client) {
        ENetEvent event;

        while (enet_host_service(client->host, &event, 0) > 0) {
                switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                        aout_client_on_connect(client, event.peer);
                        break;
                case ENET_EVENT_TYPE_RECEIVE:
                        aout_client_on_receive(client, event.peer, event.packet);
                        enet_packet_destroy(event.packet);
                        break;
                case ENET_EVENT_TYPE_DISCONNECT:
                        aout_client_on_disconnect(client, event.peer);
                        break;
                case ENET_EVENT_TYPE_NONE:
                        break;
                }
        }
}

aout_res aout_client_connect(
                aout_client* client,
                uint32_t ip,
                uint16_t port) {
        ENetAddress address;
        address.host = htonl(ip); // TODO: Make portable
        address.port = port;

        /*if (enet_address_set_host_ip(&address, ip) < 0) {
                return AOUT_CLIENT_ERR;
        }*/

        ENetPeer* peer = enet_host_connect(client->host, &address, 2, 0);

        if (!peer) {
                return AOUT_ERR(AOUT_CLIENT_ERR);
        }

        return AOUT_OK;
}

bool aout_client_is_running(
                aout_client* client) {
        return client->is_running;
}

static void aout_client_on_connect(
                aout_client* client,
                ENetPeer* peer) {
        aout_connection* connection = &client->connection;
        connection->id = peer->connectID;
        connection->peer_id = peer->outgoingPeerID;
        peer->data = connection;

        printf("connection from %u\n", connection->id);
}

static void aout_client_on_receive(
                aout_client* client,
                ENetPeer* peer,
                ENetPacket* packet) {
        (void) client;
        (void) peer;
        (void) packet;
}

static void aout_client_on_disconnect(
                aout_client* client,
                ENetPeer* peer) {
        (void) client;

        aout_connection* connection = (aout_connection*) peer->data;
        printf("disconnection from %u\n", connection->id);

        peer->data = NULL;
}
