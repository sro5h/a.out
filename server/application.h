#ifndef SERVER_APPLICATION_H
#define SERVER_APPLICATION_H

#include "server.h"
#include <common/state.h>
#include <common/tick.h>
#include <signal.h>

typedef struct aout_application aout_application;

aout_application* aout_application_new(
                void);

void aout_application_del(
                aout_application** out_self);

aout_res aout_application_run(
                aout_application* self);

void aout_application_stop(
                aout_application* self);

bool aout_application_is_running(
                aout_application* self);

#endif
