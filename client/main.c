#include "client.h"
#include <common/dummy.h>
#include <enet/enet.h>
#include <stdio.h>

int main(void) {
        if (enet_initialize() != 0) {
                fprintf(stderr, "could not initialize enet\n");
                return EXIT_FAILURE;
        }

        if (aout_dummy() == AOUT_DUMMY_OK) {
                printf("client\n");
        }

        aout_client* client = aout_client_create();

        if (!client) {
                fprintf(stderr, "could not create client\n");
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        printf("client created\n");

        if (aout_client_connect(client, 0x7f000001, 42424) != AOUT_CLIENT_OK) {
                fprintf(stderr, "could not connect to foreign host\n");
                aout_client_destroy(client);
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        while (aout_client_is_running(client)) {
                aout_client_update(client);
        }

        aout_client_destroy(client);
        enet_deinitialize();

        return EXIT_SUCCESS;
}
