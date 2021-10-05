#include "application.h"
#include "initialise.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
        if (AOUT_IS_ERR(aout_initialise())) {
                printf("error: initialisation failed\n");
                return EXIT_FAILURE;
        }

        printf("server\n");

        aout_application* app = aout_application_create();

        if (!app) {
                printf("error: could not create application\n");
                goto error;
        }

        printf("server started\n");

        aout_res res = aout_application_run(app);

        printf("server stopped\n");

        aout_application_destroy(app);
        aout_terminate();
        return AOUT_IS_OK(res) ? EXIT_SUCCESS : EXIT_FAILURE;

error:
        aout_terminate();
        return EXIT_FAILURE;
}
