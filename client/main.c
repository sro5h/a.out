#include "client.h"

#include <common/console.h>
#include <common/dummy.h>

#include <enet/enet.h>
#include <signal.h>
#include <stdio.h>

static sig_atomic_t sig_int_raised = 0;

static void cleanup(aout_client* client);
static void on_sig_int(void* context);

int main(void) {
        if (enet_initialize() != 0) {
                fprintf(stderr, "could not initialize enet\n");
                return EXIT_FAILURE;
        }

        if (AOUT_IS_OK(aout_dummy(true))) {
                printf("client\n");
        }

        aout_client* client = aout_client_create();

        if (!client) {
                fprintf(stderr, "could not create client\n");
                cleanup(NULL);
                return EXIT_FAILURE;
        }

        printf("client created\n");

        if (AOUT_IS_ERR(aout_client_connect(client, 0x7f000001, 42424))) {
                fprintf(stderr, "could not connect to foreign host\n");
                cleanup(client);
                return EXIT_FAILURE;
        }

        aout_res res = aout_on_sig_int((aout_sig_handler) {
                .callback = on_sig_int,
                .context = NULL
        });

        if (AOUT_IS_ERR(res)) {
                fprintf(stderr, "could not set SIGINT handler\n");
                cleanup(client);
                return EXIT_FAILURE;
        }

        while (aout_client_is_running(client)) {
                if (sig_int_raised) {
                        printf("\n"); // CTRL-C
                        break;
                }

                aout_client_update(client);
        }

        cleanup(client);
        printf("client destroyed\n");

        return EXIT_SUCCESS;
}

static void cleanup(aout_client* client) {
        if (client) {
                aout_client_destroy(client);
        }

        enet_deinitialize();
}

static void on_sig_int(void* context) {
        (void) context;
        sig_int_raised = 1;
}
