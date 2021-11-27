#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include "server.h"
#include <signal.h>

typedef struct cpBody cpBody;
typedef struct cpSpace cpSpace;

typedef struct aout_application {
        aout_server* server;
        cpSpace* space;
        cpBody* bodies[AOUT_SERVER_MAX_CONNECTIONS];
        bool is_running;
        double time_step;
        sig_atomic_t sigint_raised;
} aout_application;

typedef enum aout_application_res {
        AOUT_APPLICATION_OK,
        AOUT_APPLICATION_ERR
} aout_application_res;

aout_application* aout_application_create(
                void);
void aout_application_destroy(
                aout_application* app);

aout_res aout_application_run(
                aout_application* app);

void aout_application_stop(
                aout_application* app);

bool aout_application_is_running(
                aout_application* app);

#endif
