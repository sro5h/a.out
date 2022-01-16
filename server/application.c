#include "application.h"

#include <common/console.h>
#include <common/log.h>
#include <common/util.h>

#include <sokol/sokol_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLIENT_BODY_MASS   10
#define CLIENT_BODY_RADIUS 10

static void aout_application_update_fixed(
                aout_application* self,
                float64_t delta_time);

static void aout_application_update(
                aout_application* self,
                float64_t delta_time);

static void aout_application_on_connection(
                aout_server* server,
                aout_connection connection,
                void* context);

static void aout_application_on_disconnection(
                aout_server* server,
                aout_connection connection,
                void* context);

static void aout_application_on_msg_input(
                aout_server* server,
                aout_connection connection,
                aout_cl_msg_input* msg,
                void* context);

static void on_sigint(
                void* context);

aout_application* aout_application_create(
                void) {
        aout_application* self = calloc(1, sizeof(*self));

        if (!self) {
                return NULL;
        }

        self->is_running = true;
        self->time_step = 1.0 / 20;
        self->sigint_raised = 0;

        self->server = aout_server_create((aout_server_adapter) {
                .on_connection = aout_application_on_connection,
                .on_disconnection = aout_application_on_disconnection,
                .on_msg_input = aout_application_on_msg_input,
                .context = self
        }, AOUT_SERVER_MAX_CONNECTIONS);

        if (!self->server) {
                aout_loge("could not create server");
                goto error;
        }

        aout_res res = aout_on_sigint((aout_sig_handler) {
                .callback = on_sigint,
                .context = self
        });

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not set SIGINT handler");
                goto error;
        }

        return self;

error:
        aout_application_destroy(self);
        return NULL;
}

void aout_application_destroy(
                aout_application* self) {
        if (!self) {
                return;
        }

        aout_server_destroy(self->server);
        free(self);
}

aout_res aout_application_run(
                aout_application* self) {
        assert(self);

        uint64_t  last_time = stm_now();
        float64_t accumulator = 0.0;

        while (aout_application_is_running(self)) {
                if (self->sigint_raised) {
                        printf("\n"); // CTRL-C
                        aout_application_stop(self);
                        // TODO: Could also move this check to the end of loop
                        break;
                }

                const float64_t delta_time = stm_sec(stm_laptime(&last_time));

                const float64_t time_step = self->time_step;
                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
                        aout_tick_increment(&self->tick);
                        aout_application_update_fixed(self, time_step);
                }

                aout_application_update(self, delta_time);
        }

        return AOUT_OK;
}

void aout_application_stop(
                aout_application* self) {
        assert(self);
        self->is_running = false;
}

bool aout_application_is_running(
                aout_application* self) {
        assert(self);
        return self->is_running;
}

static void aout_application_update_fixed(
                aout_application* self,
                float64_t delta_time) {
        (void) delta_time;
        assert(self);

        aout_server_update(self->server);

        // Send current state to clients
        // TODO: Send state of all connected clients
        if (self->players[0].connection.id) {
                aout_server_send_msg_state(
                        self->server,
                        0,
                        &(aout_sv_msg_state) {
                                .position = self->players[0].state.p,
                        }
                );
        }

        // TODO: Send immediately
        // aout_server_flush(self->server);
}

static void aout_application_update(
                aout_application* self,
                float64_t delta_time) {
        assert(self);
        (void) delta_time;
}

static void aout_application_on_connection(
                aout_server* server,
                aout_connection connection,
                void* context) {
        assert(server); assert(context);
        aout_application* self = context;

        self->players[0] = (aout_player) {
                .connection = connection,
        };
}

static void aout_application_on_disconnection(
                aout_server* server,
                aout_connection connection,
                void* context) {
        assert(server); assert(context);
        (void) connection;
        aout_application* self = context;

        self->players[0] = (aout_player) { 0 };
}

static void aout_application_on_msg_input(
                aout_server* server,
                aout_connection connection,
                aout_cl_msg_input* msg,
                void* context) {
        assert(server); assert(msg); assert(context);
        (void) connection;
        aout_application* self = context;

        aout_vec2 direction = { 0 };

        if (msg->right) {
                direction.x += 1;
        }
        if (msg->left) {
                direction.x -= 1;
        }
        if (msg->up) {
                direction.y += 1;
        }
        if (msg->down) {
                direction.y -= 1;
        }

        aout_vec2 velocity = aout_vec2_mul(aout_vec2_norm(direction), 25);
        self->players[0].state.p = aout_vec2_add(
                self->players[0].state.p,
                velocity
        );
}

static void on_sigint(
                void* context) {
        aout_application* self = context;
        self->sigint_raised = 1;
}
