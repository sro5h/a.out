#include "client.h"

#include <common/byte_order.h>
#include <common/messages.h>
#include <enet/enet.h>
#include <stdio.h>

static void aout_client_on_connect(
                aout_client* client,
                ENetPeer* peer);

static void aout_client_on_receive(
                aout_client* client,
                ENetPeer* peer, // TODO: Maybe use peer_id or aout_connection
                ENetPacket* packet);

static void aout_client_on_receive_msg_connection(
                aout_client* client,
                aout_stream* stream);

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
        address.host = aout_hton_u32(ip);
        address.port = port;

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
        assert(client); assert(peer); assert(packet);
        assert(packet->data);

        aout_connection* connection = (aout_connection*) peer->data;
        printf("packet received from %u\n", connection->id);

        aout_stream stream = {
                .data = packet->data,
                .data_size = packet->dataLength
        };

        aout_sv_msg_type type;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_type(&stream, &type))) {
                printf("error: could not read sv_msg_type\n");
                return;
        }

        switch (type) {
        case AOUT_SV_MSG_TYPE_CONNECTION:
                aout_client_on_receive_msg_connection(client, &stream);
                break;
        default:
                printf("error: unknown sv_msg_type\n");
                break;
        }
}

static void aout_client_on_receive_msg_connection(
                aout_client* client,
                aout_stream* stream) {
        assert(client); assert(stream);

        aout_sv_msg_connection msg;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_connection(stream, &msg))) {
                printf("error: could not read sv_msg_connection\n");
                return;
        }

        printf("received sv_msg_connection: id = %u, peer_id = %u\n",
                        msg.id, msg.peer_id);
}

static void aout_client_on_disconnect(
                aout_client* client,
                ENetPeer* peer) {
        (void) client;

        aout_connection* connection = (aout_connection*) peer->data;
        printf("disconnection from %u\n", connection->id);

        peer->data = NULL;
}
