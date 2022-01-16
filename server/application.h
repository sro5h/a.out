#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include "server.h"
#include <common/state.h>
#include <common/tick.h>
#include <signal.h>

// TODO: Move to application.c and convert to static const
#define AOUT_SERVER_MAX_CONNECTIONS 10

typedef struct aout_player {
        aout_connection connection;
        aout_state state;
} aout_player;

typedef struct aout_application {
        aout_player players[AOUT_SERVER_MAX_CONNECTIONS];
        aout_server* server;
        double time_step;
        aout_tick tick;
        bool is_running;
        sig_atomic_t sigint_raised;
} aout_application;

typedef enum aout_application_res {
        AOUT_APPLICATION_OK,
        AOUT_APPLICATION_ERR
} aout_application_res;

aout_application* aout_application_create(
                void);
void aout_application_destroy(
                aout_application* self);

aout_res aout_application_run(
                aout_application* self);

void aout_application_stop(
                aout_application* self);

bool aout_application_is_running(
                aout_application* self);

#endif
