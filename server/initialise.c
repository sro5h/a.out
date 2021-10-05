#include "initialise.h"

#include <enet/enet.h>
#include <sokol/sokol_time.h>

aout_res aout_initialise(
                void) {
        stm_setup();

        if (enet_initialize() != 0) {
                return AOUT_ERR(AOUT_INITIALISE_ERR_ENET);
        }

        return AOUT_OK;
}

void aout_terminate(
                void) {
        enet_deinitialize();
}
