#include "application.h"
#include "player_mesh.h"
#include "prediction.h"

#include <common/console.h>
#include <common/log.h>
#include <common/movement.h>
#include <common/util.h>

#include <chipmunk/chipmunk.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <sokol/sokol_time.h>
#include <stdlib.h>

static void aout_application_update_fixed(
                aout_application* self,
                float64_t delta_time);

static void aout_application_update(
                aout_application* self,
                float64_t delta_time,
                float64_t alpha);

static void aout_application_on_connection(
                aout_client* client,
                void* context);

static void aout_application_on_disconnection(
                aout_client* client,
                void* context);

static void aout_application_on_msg_state(
                aout_client* client,
                aout_sv_msg_state* msg,
                void* context);

static void aout_application_reconcile(
                aout_application* self,
                aout_tick tick,
                aout_state_full const* server_state);

static void on_sigint(
                void* context);

// TODO: add aout_application_desc
aout_application* aout_application_create(
                //aout_application_desc* desc
                void) {
        aout_application* self = calloc(1, sizeof(*self));

        if (!self) {
                return NULL;
        }

        size_t const tick_rate = 32;
        self->is_running = true;
        self->is_connected = false;
        self->time_step = 1.0 / tick_rate;
        self->sigint_raised = 0;

        self->space = cpSpaceNew();

        if (!self->space) {
                aout_loge("could not create space");
                goto error;
        }

        cpSpaceSetIterations(self->space, 10);
        cpSpaceSetSleepTimeThreshold(self->space, 0.5f);

        self->player_body = cpSpaceAddBody(self->space, cpBodyNew(
                10,
                cpMomentForCircle(10, 0, 10, cpvzero)
        ));

        if (!self->player_body) {
                aout_loge("could not create player body");
                goto error;
        }

        cpBodySetPosition(self->player_body, cpvzero);

        cpShape* shape = cpSpaceAddShape(self->space, cpCircleShapeNew(
                self->player_body,
                10,
                cpvzero
        ));

        if (!shape) {
                aout_loge("could not create shape");
                goto error;
        }

        cpShapeSetElasticity(shape, 0.0f);
        cpShapeSetFriction(shape, 0.7f);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        self->window = glfwCreateWindow(640, 480, "client", NULL, NULL);

        if (!self->window) {
                aout_loge("could not create window");
                goto error;
        }

        glfwSetWindowPos(self->window, 0, 0);
        glfwMakeContextCurrent(self->window);
        glfwSwapInterval(1);

        self->renderer = aout_renderer_create();

        if (!self->renderer) {
                aout_loge("could not create renderer");
                goto error;
        }

        aout_renderer_set_view(self->renderer, 640, 480);
        self->player_mesh = aout_player_mesh_create(
                (aout_rgba8) { 0xf6, 0x08, 0x1e, 0xff }
        );
        self->server_mesh = aout_player_mesh_create(
                (aout_rgba8) { 0x31, 0x93, 0xd8, 0xff }
        );

        self->client = aout_client_create((aout_client_adapter) {
                .on_connection = aout_application_on_connection,
                .on_disconnection = aout_application_on_disconnection,
                .on_msg_state = aout_application_on_msg_state,
                .context = self
        });

        if (!self->client) {
                aout_loge("could not create client");
                goto error;
        }

        // Move somewhere else
        if (AOUT_IS_ERR(aout_client_connect(self->client, 0x7f000001, 42424))) {
                aout_loge("could not connect to foreign host");
                goto error;
        }

        self->predictions = aout_ring_create(tick_rate, sizeof(aout_prediction));

        if (!self->predictions) {
                aout_loge("could not create prediction buffer");
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

        // As all pointers are zero initialised, simply call
        // aout_application_destroy to release all allocated resources.
        // NULL pointers will be ignored.
error:
        aout_application_destroy(self);
        return NULL;
}

void aout_application_destroy(
                aout_application* self) {
        if (!self) {
                return;
        }

        // NULL is safely handled by *_destroy
        //assert(self->window); assert(self->client);

        aout_ring_destroy(self->predictions);
        aout_client_destroy(self->client);
        aout_renderer_destroy(self->renderer);
        glfwDestroyWindow(self->window);
        aout_space_free(self->space);
        free(self);
}

aout_res aout_application_run(
                aout_application* self) {
        assert(self);

        uint64_t last_time = stm_now();
        double accumulator = 0.0;

        while (aout_application_is_running(self)) {
                if (glfwWindowShouldClose(self->window) || self->sigint_raised) {
                        if (self->sigint_raised) { printf("\n"); } // CTRL-C
                        aout_application_stop(self);
                        // TODO: Maybe move check to end of loop
                        break;
                }

                uint64_t  const now = stm_now();
                float64_t const delta_time = stm_sec(stm_diff(now, last_time));
                last_time = now;

                float64_t const time_step = self->time_step;
                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
                        aout_tick_increment(&self->tick);
                        aout_application_update_fixed(self, time_step);
                }

                float64_t const alpha = accumulator / time_step;
                aout_application_update(self, delta_time, alpha);
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
        assert(self);
        (void) delta_time;

        aout_client_update(self->client);

        glfwPollEvents();

        //if (!aout_tick_filter_rate(&self->tick, 2)) { return; }

        aout_input input = { 0 };
        input.right = glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS;
        input.left  = glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS;
        input.up    = glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS;
        input.down  = glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS;

        aout_movement_apply(self->player_body, &input);
        cpSpaceStep(self->space, self->time_step);

        self->player_state_prev = self->player_state;
        self->player_state = aout_state_full_from_body(self->player_body);

        if (self->is_connected) {
                aout_cl_msg_input msg = {
                        .tick  = self->tick,
                        .input = input
                };

                aout_client_send_msg_input(self->client, &msg);
        }

        aout_client_flush(self->client);
}

static void aout_application_update(
                aout_application* self,
                float64_t delta_time,
                float64_t alpha) {
        assert(self);
        (void) delta_time;

        aout_transform interpolated = { 0 };
        /*interpolated = aout_transform_add(
                aout_transform_mul(self->player_transform, alpha),
                aout_transform_mul(self->player_transform_prev, 1.0 - alpha)
        );*/
        interpolated.position = self->player_state.p;
        interpolated.rotation = self->player_state.r;
        interpolated.scale = (aout_vec2) { .x = 1.0f, .y = 1.0f };

        aout_transform server_trans = {
                .position = self->server_state.p,
                .rotation = self->server_state.r,
                .scale = (aout_vec2) { .x = 1.0f, .y = 1.0f }
        };

        int width, height;
        glfwGetFramebufferSize(self->window, &width, &height);
        assert(width > 0); assert(height > 0);

        aout_renderer_begin(self->renderer, width, height);

        aout_renderer_render_mesh(
                self->renderer,
                &self->server_mesh,
                &server_trans
        );

        aout_renderer_render_mesh(
                self->renderer,
                &self->player_mesh,
                &interpolated
        );

        aout_renderer_end(self->renderer);

        glfwSwapBuffers(self->window);
}

static void aout_application_on_connection(
                aout_client* client,
                void* context) {
        assert(client); assert(context);
        aout_application* self = context;

        assert(!self->is_connected);

        self->is_connected = true;
}

static void aout_application_on_disconnection(
                aout_client* client,
                void* context) {
        assert(client); assert(context);
        aout_application* self = context;

        assert(self->is_connected);

        self->is_connected = false;
}

static void aout_application_on_msg_state(
                aout_client* client,
                aout_sv_msg_state* msg,
                void* context) {
        assert(client); assert(msg); assert(context);
        aout_application* self = context;

        assert(self->is_connected);

        self->server_state = msg->state;

        if (aout_ring_empty(self->predictions)) {
                self->player_state_prev = self->player_state;
                self->player_state = self->server_state;
        } else {
                aout_application_reconcile(self, msg->tick, &self->server_state);
        }
}

static void aout_application_reconcile(
                aout_application* self,
                aout_tick tick,
                aout_state_full const* server_state) {
        assert(self); assert(server_state);

        // TODO: Maybe first do a sanity check whether tick is less than
        // back.tick?

        while (!aout_ring_empty(self->predictions)) {
                aout_prediction const* p = aout_ring_front(self->predictions);

                if (aout_tick_cmp(p->tick, tick) < 0) {
                        aout_ring_pop_front(self->predictions);
                } else {
                        break;
                }
        }

        assert(!aout_ring_empty(self->predictions));

        aout_prediction const* front = aout_ring_front(self->predictions);
        assert(aout_tick_cmp(front->tick, tick) == 0);

        //if (!aout_state_full_eql(&front->state, server_state)) {
                // Front shouldn't be calculated, as it is already in the
                // server_state
                aout_ring_pop_front(self->predictions);

                // TODO: Maybe sent fat state (i.e. also velocity, force etc.)
                // to each connected client for its own body
                // Bodies of other players don't need as detailed state as they
                // aren't predicted.

                // Reset physics scene
                // TODO: Will this even get applied? Won't it be simply
                // overwritten by aout_apply_movement? Talking about velocity, force
                cpBodySetPosition(self->player_body, cpv(server_state->p.x, server_state->p.y));
                cpBodySetVelocity(self->player_body, cpv(server_state->v.x, server_state->v.y));
                cpBodySetForce(self->player_body, cpv(server_state->f.x, server_state->f.y));

                // TODO: Maybe use aout_ring_foreach();
                for (size_t i = 0; i < aout_ring_size(self->predictions); ++i) {
                        aout_prediction* p = aout_ring_at(self->predictions, i);

                        aout_movement_apply(self->player_body, &p->input);
                        cpSpaceStep(self->space, self->time_step);

                        aout_state_full reconciled = aout_state_full_from_body(
                                self->player_body
                        );
                        p->state = reconciled;
                }

                // Apply the final state
                // TODO: How should this be done correctly? Keep in mind that
                // aout_application_reconcile can be called multiple times.
                // For now keep self->player_state_prev and only update
                // self->player_state
                aout_prediction const* p = aout_ring_back(self->predictions);
                self->player_state = p->state;
        /*} else {
                // Prediction was correct
                aout_ring_pop_front(self->predictions);
        }*/
}

static void on_sigint(
                void* context) {
        aout_application* self = context;
        self->sigint_raised = 1;
}
