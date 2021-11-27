#include "client.h"

#include <common/byte_order.h>
#include <common/log.h>
#include <common/messages.h>
#include <enet/enet.h>

static void aout_client_on_connect(
                aout_client* client,
                ENetPeer* peer);

static void aout_client_on_receive(
                aout_client* client,
                ENetPeer* peer, // TODO: Maybe use peer_id or aout_connection
                ENetPacket* packet);

static void aout_client_on_receive_msg_connection(
                aout_client* client,
                aout_connection* connection,
                aout_stream* stream);

static void aout_client_on_receive_msg_state(
                aout_client* client,
                aout_connection* connection,
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
        if (client) {
                enet_host_destroy(client->host);
                free(client);
        }
}

void aout_client_update(
                aout_client* client) {
        assert(client);

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
        assert(client);

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
        assert(client);
        return client->is_running;
}

static void aout_client_on_connect(
                aout_client* client,
                ENetPeer* peer) {
        assert(client); assert(peer);

        aout_connection* connection = &client->connection;
        connection->id = peer->connectID;
        connection->peer_id = peer->outgoingPeerID;
        peer->data = connection;

        aout_logd("[0x%08x] connection", connection->id);
}

static void aout_client_on_receive(
                aout_client* client,
                ENetPeer* peer,
                ENetPacket* packet) {
        assert(client); assert(peer); assert(packet);
        assert(packet->data);

        aout_connection* connection = (aout_connection*) peer->data;
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
                aout_client_on_receive_msg_connection(client, connection,
                                &stream);
                break;
        case AOUT_SV_MSG_TYPE_STATE:
                aout_client_on_receive_msg_state(client, connection, &stream);
                break;
        default:
                aout_loge("unknown sv_msg_type");
                break;
        }
}

static void aout_client_on_receive_msg_connection(
                aout_client* client,
                aout_connection* connection,
                aout_stream* stream) {
        assert(client); assert(connection); assert(stream);

        aout_sv_msg_connection msg;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_connection(stream, &msg))) {
                aout_loge("could not read sv_msg_connection");
                return;
        }

        aout_logd("[0x%08x] sv_msg_connection received: ", connection->id);
        aout_logd("{ .id = 0x%x, .peer_id = 0x%x }", msg.id, msg.peer_id);
}

static void aout_client_on_receive_msg_state(
                aout_client* client,
                aout_connection* connection,
                aout_stream* stream) {
        assert(client); assert(connection); assert(stream);

        aout_sv_msg_state msg;
        if (AOUT_IS_ERR(aout_stream_read_sv_msg_state(stream, &msg))) {
                aout_loge("could not read sv_msg_state");
                return;
        }

        aout_logd("[0x%08x] sv_msg_state received: ", connection->id);
        aout_logd("{ .position = { %f, %f } }", msg.position.x, msg.position.y);
}

static void aout_client_on_disconnect(
                aout_client* client,
                ENetPeer* peer) {
        assert(client); assert(peer);
        assert(peer->data);

        aout_connection* connection = (aout_connection*) peer->data;
        aout_logd("[0x%08x] disconnection", connection->id);

        peer->data = NULL;
}
