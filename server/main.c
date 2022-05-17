#include "application.h"
#include "initialise.h"

#include <common/log.h>

#include <stdlib.h>

int main(void) {
        if (AOUT_IS_ERR(aout_initialise())) {
                aout_loge("initialisation failed");
                return EXIT_FAILURE;
        }

        aout_logd("server");

        aout_application* app = aout_application_new();

        aout_logd("server started");

        aout_res res = aout_application_run(app);

        aout_logd("server stopped");

        aout_application_del(&app);
        aout_terminate();

        return AOUT_IS_OK(res) ? EXIT_SUCCESS : EXIT_FAILURE;
}
