#include "application.h"

#include <common/console.h>
#include <common/log.h>
#include <common/movement.h>
#include <common/util.h>

#include <chipmunk/chipmunk.h>
#include <sokol/sokol_time.h>
#include <stdio.h>
#include <string.h>

#define CLIENT_BODY_MASS   10
#define CLIENT_BODY_RADIUS 10

static void aout_application_update_fixed(
                aout_application* self,
                double delta_time);

static void aout_application_update(
                aout_application* self,
                double delta_time);

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
        // Bodies must be zero
        aout_application* self = calloc(1, sizeof(*self));

        if (!self) {
                return NULL;
        }

        self->is_running = true;
        self->time_step = 1.0 / 32;
        self->sigint_raised = 0;

        self->space = cpSpaceNew();

        if (!self->space) {
                aout_loge("could not create space");
                goto error;
        }

        cpSpaceSetIterations(self->space, 10);
        cpSpaceSetSleepTimeThreshold(self->space, 0.5f);
        // TODO: Create physics scene

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
        aout_space_free(self->space);
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

                float64_t const delta_time = stm_sec(stm_laptime(&last_time));

                float64_t const time_step = self->time_step;
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
                double delta_time) {
        assert(self);
        self->received = false;

        // Reset input, could also replay last input in case non was received
        if (self->players[0].body) {
                self->players[0].last_input = (aout_input) { 0 };
                // Maybe should advance last_input_tick by one
                self->players[0].last_input_tick = (aout_tick) { 0 };
        }

        aout_server_update(self->server);

        if (self->players[0].body) {
                aout_movement_apply(
                        self->players[0].body,
                        &self->players[0].last_input
                );
        }

        cpSpaceStep(self->space, delta_time);

        // Send current state to clients
        // TODO: Send state of all connected clients
        if (self->players[0].body) {
                aout_sv_msg_state msg = { 0 };
                // TODO: Send tick from input message whose input was applied
                msg.tick = self->players[0].last_input_tick;
                msg.state = aout_state_full_from_body(self->players[0].body);

                aout_server_send_msg_state(self->server, 0, &msg);
        }

        aout_logd("TICK");

        aout_server_flush(self->server);
}

static void aout_application_update(
                aout_application* self,
                double delta_time) {
        assert(self);
        (void) delta_time;
}

static void aout_application_on_connection(
                aout_server* server,
                aout_connection connection,
                void* context) {
        assert(server); assert(context);
        aout_application* self = context;

        assert(!self->players[connection.peer_id].body);

        cpBody* body = cpSpaceAddBody(self->space, cpBodyNew(
                CLIENT_BODY_MASS,
                cpMomentForCircle(
                        CLIENT_BODY_MASS,
                        0,
                        CLIENT_BODY_RADIUS,
                        cpvzero
                )
        ));

        cpBodySetPosition(body, cpvzero);

        cpShape* shape = cpSpaceAddShape(self->space, cpCircleShapeNew(
                body,
                CLIENT_BODY_RADIUS,
                cpvzero
        ));
        cpShapeSetElasticity(shape, 0.0f);
        cpShapeSetFriction(shape, 0.7f);

        self->players[connection.peer_id].body = body;
}

static void aout_application_on_disconnection(
                aout_server* server,
                aout_connection connection,
                void* context) {
        assert(server); assert(context);
        aout_application* self = context;

        aout_player* player = &self->players[connection.peer_id];
        assert(player->body);

        // Don't use post step callbacks as this will never be called inside
        // cpSpaceStep.
        aout_body_free(player->body);
        player->body = NULL;
}

static void aout_application_on_msg_input(
                aout_server* server,
                aout_connection connection,
                aout_cl_msg_input* msg,
                void* context) {
        assert(server); assert(msg); assert(context);
        (void) connection;
        aout_application* self = context;

        assert(self->players[0].body);
        assert(!self->received);

        // TODO: Should only be called once per tick (i.e. in update_fixed)
        // Simply store received input in self->players[0].last_input
        // Could buffer inputs to combat latency variation, but this will have
        // the effect that the latency between client and server is fixed to
        // the latency of the first input.
        // Could fix this by having a flexible 'target' latency based on some
        // kind of average of the last n latencies. Would mean though that some
        // inputs will be skipped...
        // Could also synchronise the client latency on connection to the server
        // but then there is a problem if the latency increases and thus all
        // inputs suddenly arrive to late.
        // Maybe a buffer of 2-4 input packets is best, so that sudden spikes
        // in latency get smoothed out (at least to some degree). Will increase
        // latency and the amount of lag compensation for hit detection though.
        //aout_movement_apply(self->players[0].body, &msg->input);
        self->players[0].last_input = msg->input;
        self->players[0].last_input_tick = msg->tick;
        self->received = true;
}

static void on_sigint(
                void* context) {
        aout_application* self = context;
        self->sigint_raised = 1;
}
