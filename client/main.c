#include "game.h"

#include <common/result.h>

#include <enet/enet.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>

typedef enum aout_initialise_res {
        AOUT_INITIALISE_OK,
        AOUT_INITIALISE_ERR_ENET,
        AOUT_INITIALISE_ERR_GLFW,
} aout_initialise_res;

static aout_res aout_initialise(
                void);

static void aout_terminate(
                void);

int main(void) {
        if (AOUT_IS_ERR(aout_initialise())) {
                printf("error: initialisation failed\n");
                return EXIT_FAILURE;
        }

        printf("client\n");

        aout_game* game = aout_game_create();

        if (!game) {
                printf("error: could not create game\n");
                goto error;
        }

        printf("client started\n");

        aout_res res = aout_game_run(game);

        printf("client stopped\n");

        aout_game_destroy(game);
        aout_terminate();
        return AOUT_IS_OK(res) ? EXIT_SUCCESS : EXIT_FAILURE;

error:
        aout_terminate();
        return EXIT_FAILURE;
}

static aout_res aout_initialise(
                void) {
        if (enet_initialize() != 0) {
                return AOUT_ERR(AOUT_INITIALISE_ERR_ENET);
        }

        if (!glfwInit()) {
                enet_deinitialize();
                return AOUT_ERR(AOUT_INITIALISE_ERR_GLFW);
        }

        return AOUT_OK;
}

static void aout_terminate(
                void) {
        glfwTerminate();
        enet_deinitialize();
}
