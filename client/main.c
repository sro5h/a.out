#include "application.h"
#include "initialise.h"

#include <common/log.h>

#include <stdlib.h>

int main(void) {
        if (AOUT_IS_ERR(aout_initialise())) {
                aout_loge("initialisation failed");
                return EXIT_FAILURE;
        }

        aout_logd("client");

        aout_application* app = aout_application_create();

        if (!app) {
                aout_loge("could not create application");
                goto error;
        }

        aout_logd("client started");

        aout_res res = aout_application_run(app);

        aout_logd("client stopped");

        aout_application_destroy(app);
        aout_terminate();
        return AOUT_IS_OK(res) ? EXIT_SUCCESS : EXIT_FAILURE;

error:
        aout_terminate();
        return EXIT_FAILURE;
}
