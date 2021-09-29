#include "server.h"

#include <common/console.h>
#include <common/dummy.h>

#include <enet/enet.h>
#include <signal.h>
#include <stdio.h>

static sig_atomic_t sig_int_raised = 0;

static void cleanup(aout_server* server);
static void on_sig_int(void* context);

int main(void) {
        if (enet_initialize() != 0) {
                fprintf(stderr, "could not initialize enet\n");
                return EXIT_FAILURE;
        }

        if (AOUT_IS_ERR(aout_dummy(false))) {
                printf("server\n");
        }

        aout_server* server = aout_server_create();

        if (!server) {
                fprintf(stderr, "could not create server\n");
                cleanup(NULL);
                return EXIT_FAILURE;
        }

        printf("server created\n");

        aout_res res = aout_on_sig_int((aout_sig_handler) {
                .callback = on_sig_int,
                .context = NULL
        });

        if (AOUT_IS_ERR(res)) {
                fprintf(stderr, "could not set SIGINT handler\n");
                cleanup(server);
                return EXIT_FAILURE;
        }

        while (aout_server_is_running(server)) {
                if (sig_int_raised) {
                        printf("\n"); // CTRL-C
                        break;
                }

                aout_server_update(server);
        }

        cleanup(server);
        printf("server destroyed\n");

        return EXIT_SUCCESS;
}

static void cleanup(aout_server* server) {
        if (server) {
                aout_server_destroy(server);
        }

        enet_deinitialize();
}

static void on_sig_int(void* context) {
        (void) context;
        sig_int_raised = 1;
}
