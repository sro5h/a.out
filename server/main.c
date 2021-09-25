#include "server.h"
#include <common/dummy.h>
#include <enet/enet.h>
#include <stdio.h>

int main(void) {
        if (enet_initialize() != 0) {
                fprintf(stderr, "could not initialize enet\n");
                return EXIT_FAILURE;
        }

        if (aout_dummy() == AOUT_DUMMY_OK) {
                printf("server\n");
        }

        aout_server* server = aout_server_create();

        if (!server) {
                fprintf(stderr, "could not create server\n");
                enet_deinitialize();
                return EXIT_FAILURE;
        }

        printf("server created\n");

        while (aout_server_is_running(server)) {
                aout_server_update(server);
        }

        aout_server_destroy(server);
        enet_deinitialize();

        return EXIT_SUCCESS;
}
