#include "application.h"

#include <common/console.h>
#include <common/log.h>
#include <common/movement.h>
#include <common/util.h>

#include <chipmunk/chipmunk.h>
#include <sokol/sokol_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOUT_SERVER_REAPPLY_LAST_INPUT

#define CLIENT_BODY_MASS   10
#define CLIENT_BODY_RADIUS 10
#define SERVER_MAX_CONNECTIONS 10

typedef struct aout_player {
        aout_connection connection;
        cpBody* body;
        aout_state state;
        aout_cl_msg_input last_msg_input;
        aout_tick tick; // Should correspond to state
} aout_player;

typedef struct aout_application {
        aout_player players[SERVER_MAX_CONNECTIONS];
        size_t player_count;
        aout_server* server;
        cpSpace* space;
        double time_step;
        aout_tick tick;
        bool is_running;
        sig_atomic_t sigint_raised;
} aout_application;


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

        self->player_count = 0;
        self->is_running = true;
        self->time_step = 1.0 / 20;
        self->sigint_raised = 0;

        self->space = cpSpaceNew();

        if (!self->space) {
                aout_loge("could not create space");
                goto error;
        }

        cpSpaceSetIterations(self->space, 10);
        cpSpaceSetSleepTimeThreshold(self->space, 0.5f);

        self->server = aout_server_create((aout_server_adapter) {
                .on_connection = aout_application_on_connection,
                .on_disconnection = aout_application_on_disconnection,
                .on_msg_input = aout_application_on_msg_input,
                .context = self
        }, SERVER_MAX_CONNECTIONS);

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

                const float64_t delta_time = stm_sec(stm_laptime(&last_time));

                const float64_t time_step = self->time_step;
                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
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

        self->tick = aout_tick_increment(self->tick, 1);

        if (self->player_count > 0) {
                // TODO: Should do for all players
                self->players[0].tick = aout_tick_increment(self->players[0].tick, 1);
        }

        aout_server_update(self->server);

        // TODO: Remove
        if (self->player_count == 0) {
                return;
        }

        aout_player* player = &self->players[0];

        // First skip to oldest input in lastest input msg
        if (aout_tick_cmp(player->tick, player->last_msg_input.tick) < 0) {
                player->tick = player->last_msg_input.tick;
        }

        // Check whether the expected input is old enough to be in the latest
        // input msg
        size_t index = aout_tick_diff(player->tick, player->last_msg_input.tick);
        if (index < AOUT_CL_MSG_INPUT_BUFFER_COUNT) {
                // TODO: Remove
                /*aout_state_apply_input(
                        &player->state,
                        &player->last_msg_input.inputs[index]
                );*/
                aout_body_apply_input(
                        player->body,
                        &player->last_msg_input.inputs[index]
                );
        }
        //} else {
        // Maybe replay last input
        //}

        cpSpaceStep(self->space, delta_time);
        player->state = aout_state_from_body(player->body);

        // Send current state to clients
        // TODO: Send state of all connected clients
        if (player->connection.id) {
                aout_server_send_msg_state(
                        self->server,
                        0,
                        &(aout_sv_msg_state) {
                                .tick = player->tick,
                                .state = player->state
                        }
                );
        }

        aout_server_flush(self->server);
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
                .body = cpSpaceAddBody(self->space, cpBodyNew(
                        10,
                        cpMomentForCircle(
                                10,
                                0,
                                10,
                                cpvzero
                        )
                )),
        };

        cpBodySetPosition(self->players[0].body, cpvzero);
        self->player_count = 1;

        cpShape* shape = cpSpaceAddShape(self->space, cpCircleShapeNew(
                self->players[0].body,
                10,
                cpvzero
        ));
        cpShapeSetElasticity(shape, 0.0f);
        cpShapeSetFriction(shape, 0.7f);
}

static void aout_application_on_disconnection(
                aout_server* server,
                aout_connection connection,
                void* context) {
        assert(server); assert(context);
        (void) connection;
        aout_application* self = context;

        aout_body_free(self->players[0].body);
        self->players[0] = (aout_player) { 0 };
        self->player_count = 0;
}

static void aout_application_on_msg_input(
                aout_server* server,
                aout_connection connection,
                aout_cl_msg_input* msg,
                void* context) {
        assert(server); assert(msg); assert(context);
        (void) connection;
        aout_application* self = context;

        // Only store the latest received input message
        aout_player* player = &self->players[0];
        if (aout_tick_cmp(msg->tick, player->last_msg_input.tick) > 0) {
                player->last_msg_input = *msg;
        }
}

static void on_sigint(
                void* context) {
        aout_application* self = context;
        self->sigint_raised = 1;
}
