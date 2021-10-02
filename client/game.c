#include "game.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

static void aout_game_update_fixed(
                aout_game* game,
                double delta_time);

static void aout_game_update(
                aout_game* game,
                double delta_time);

// TODO: add aout_game_desc
aout_game* aout_game_create(
                //aout_game_desc* desc
                void) {
        aout_game* game = malloc(sizeof(*game));

        if (!game) {
                return NULL;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        game->window = glfwCreateWindow(640, 480, "client", NULL, NULL);

        if (!game->window) {
                printf("error: could not create window\n");
                goto error_window;
        }

        glfwSetWindowPos(game->window, 0, 0);
        glfwMakeContextCurrent(game->window);
        glfwSwapInterval(1);

        game->client = aout_client_create();

        if (!game->client) {
                printf("error: could not create client\n");
                goto error_client;
        }

        game->is_running = true;
        game->time_step = 1.0 / 64;

        // Move somewhere else
        if (AOUT_IS_ERR(aout_client_connect(game->client, 0x7f000001, 42424))) {
                printf("error: could not connect to foreign host\n");
                goto error_connect;
        }

        return game;

error_connect:
        aout_client_destroy(game->client);

error_client:
        glfwDestroyWindow(game->window);

error_window:
        free(game);
        return NULL;
}

void aout_game_destroy(
                aout_game* game) {
        if (!game) {
                return;
        }

        assert(game->window); assert(game->client);

        aout_client_destroy(game->client);
        glfwDestroyWindow(game->window);
        free(game);
}

aout_res aout_game_run(
                aout_game* game) {
        assert(game);

        double last_time = glfwGetTime();
        double accumulator = 0.0;

        while (aout_game_is_running(game)) {
                if (glfwWindowShouldClose(game->window)) {
                        aout_game_stop(game);
                }

                const double now = glfwGetTime();
                const double delta_time = now - last_time;
                last_time = now;

                const double time_step = game->time_step;
                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
                        glfwPollEvents();
                        aout_game_update_fixed(game, time_step);
                }

                aout_game_update(game, delta_time);
        }

        return AOUT_OK;
}

void aout_game_stop(
                aout_game* game) {
        assert(game);
        game->is_running = false;
}

bool aout_game_is_running(
                aout_game* game) {
        assert(game);
        return game->is_running;
}

static void aout_game_update_fixed(
                aout_game* game,
                double delta_time) {
        assert(game);
        (void) delta_time;
        aout_client_update(game->client);
}

static void aout_game_update(
                aout_game* game,
                double delta_time) {
        assert(game);
        (void) delta_time;
}
