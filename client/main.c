#include <stdbool.h>
#include <stdio.h>
#include <common/dummy.h>
#include <enet/enet.h>

typedef struct aout_peer_info {
        uint32_t connection_id;
} aout_peer_info;

void on_connect(ENetPeer* peer);
void on_receive(ENetPeer* peer, ENetPacket* packet);
void on_disconnect(ENetPeer* peer);

int main(void) {
        if (enet_initialize() != 0) {
                fprintf(stderr, "Could not initialize enet\n");
                return EXIT_FAILURE;
        }

        if (aout_dummy() == AOUT_DUMMY_OK) {
                printf("client\n");
        }

        ENetHost* host = enet_host_create(NULL, 1, 2, 0, 0);

        if (!host) {
                fprintf(stderr, "Could not create host\n");
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        ENetAddress address;
        enet_address_set_host(&address, "localhost");
        address.port = 42424;

        ENetPeer* peer = enet_host_connect(host, &address, 2, 0);

        if (!peer) {
                fprintf(stderr, "Could not connect to foreign host\n");
                enet_host_destroy(host);
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        while (true) {
                ENetEvent event;

                while (enet_host_service(host, &event, 0) > 0) {
                        switch (event.type) {
                        case ENET_EVENT_TYPE_CONNECT:
                                on_connect(event.peer);
                                break;
                        case ENET_EVENT_TYPE_RECEIVE:
                                on_receive(event.peer, event.packet);
                                enet_packet_destroy(event.packet);
                                break;
                        case ENET_EVENT_TYPE_DISCONNECT:
                                on_disconnect(event.peer);
                                break;
                        case ENET_EVENT_TYPE_NONE:
                                fprintf(stderr, "Invalid event\n");
                                break;
                        }
                }
        }

        enet_host_destroy(host);
        enet_deinitialize();

        return EXIT_SUCCESS;
}

void on_connect(ENetPeer* peer) {
        aout_peer_info* peer_info = malloc(sizeof(*peer_info));
        peer_info->connection_id = peer->connectID;
        peer->data = peer_info;

        printf("Connection from %u at %x:%u\n",
                peer_info->connection_id,
                peer->address.host,
                peer->address.port);
}

void on_receive(ENetPeer* peer, ENetPacket* packet) {
        aout_peer_info* peer_info = (aout_peer_info*) peer->data;

        printf("Received %s from %u\n",
                packet->data,
                peer_info->connection_id);
}

void on_disconnect(ENetPeer* peer) {
        aout_peer_info* peer_info = (aout_peer_info*) peer->data;

        printf("Disconnection from %u at %x:%u\n",
                peer_info->connection_id,
                peer->address.host,
                peer->address.port);

        free(peer_info); // TODO: Fix leak if on_disconnect is never called
        peer->data = NULL;
}
