#include "initialise.h"

#include <enet/enet.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <sokol/sokol_time.h>

aout_res aout_initialise(
                void) {
        stm_setup();

        if (enet_initialize() != 0) {
                return AOUT_ERR(AOUT_INITIALISE_ERR_ENET);
        }

        if (!glfwInit()) {
                enet_deinitialize();
                return AOUT_ERR(AOUT_INITIALISE_ERR_GLFW);
        }

        return AOUT_OK;
}

void aout_terminate(
                void) {
        glfwTerminate();
        enet_deinitialize();
}
