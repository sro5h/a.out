#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "client.h"

typedef struct GLFWwindow GLFWwindow;

typedef struct aout_game {
        aout_client* client;
        GLFWwindow* window;
        bool is_running;
        double time_step; // Maybe use ticks_per_second instead
} aout_game; // aout_cl_game // TODO: Maybe rename to aout_cl_app

typedef enum aout_game_res {
        AOUT_GAME_OK,
        AOUT_GAME_ERR
} aout_game_res;

aout_game* aout_game_create(
                void);

void aout_game_destroy(
                aout_game* game);

aout_res aout_game_run(
                aout_game* game);

void aout_game_stop(
                aout_game* game);

bool aout_game_is_running(
                aout_game* game);

#endif
