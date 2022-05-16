#include "application.h"
#include "debug_draw_glue.h"
#include "mesh_player.h"
#include "prediction.h"

#include <common/console.h>
#include <common/log.h>
#include <common/movement.h>
#include <common/ring.h>
#include <common/state.h>
#include <common/util.h>

#include <chipmunk/chipmunk.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <sokol/sokol_time.h>
#include <stdlib.h>

typedef struct aout_application {
        GLFWwindow* window;
        aout_debug_draw* debug_draw;
        aout_renderer* renderer;
        aout_mesh mesh_player;
        aout_mesh mesh_server;
        bool is_running;

        cpSpace* space;
        cpBody* body;

        aout_client* client;
        bool is_connected;

        double time_step; // Maybe use ticks_per_second instead
        aout_tick tick;

        sig_atomic_t sigint_raised;
        aout_state state_server;
        aout_state state;
        aout_state state_prev;
        aout_ring* predictions;
} aout_application;


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

static void aout_application_send_msg_input(
                aout_application* self);

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

        size_t const tick_rate = 20;
        self->is_running = true;
        self->is_connected = false;
        self->time_step = 1.0 / tick_rate;
        self->sigint_raised = 0;

        self->predictions = aout_ring_new(tick_rate, sizeof(aout_prediction));

        if (!self->predictions) {
                aout_loge("could not create predictions ring");
                goto error;
        }

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
        // Disable vsync to fix update_fixed being called twice after another.
        // Maybe add a separate rendering thread in the future so that vsync
        // can be enabled.
        glfwSwapInterval(0);

        self->renderer = aout_renderer_create();

        if (!self->renderer) {
                aout_loge("could not create renderer");
                goto error;
        }

        aout_renderer_set_view(self->renderer, 640, 480);
        self->mesh_player = aout_mesh_player_new((aout_rgba8) {
                0x74, 0x00, 0xB8, 0xff
        });
        self->mesh_server = aout_mesh_player_new((aout_rgba8) {
                0x5e, 0x60, 0xce, 0xff
        });

        self->debug_draw = aout_debug_draw_new();
        aout_debug_draw_set_view(self->debug_draw, 640, 480);

        self->space = cpSpaceNew();

        if (!self->space) {
                aout_loge("could not create space");
                goto error;
        }

        cpSpaceSetIterations(self->space, 10);
        cpSpaceSetSleepTimeThreshold(self->space, 0.5f);

        // Create player body
        self->body = cpSpaceAddBody(self->space, cpBodyNew(
                1,
                cpMomentForCircle(
                        1,
                        0,
                        50,
                        cpvzero
                )
        ));
        cpBodySetVelocityUpdateFunc(self->body, aout_body_velocity_update);
        cpBodySetPosition(self->body, cpvzero);

        cpShape* shape = cpSpaceAddShape(self->space, cpCircleShapeNew(
                self->body,
                50,
                cpvzero
        ));
        cpShapeSetElasticity(shape, 0.0f);
        cpShapeSetFriction(shape, 0.7f);

        self->client = aout_client_new((aout_client_adapter) {
                .on_connection = aout_application_on_connection,
                .on_disconnection = aout_application_on_disconnection,
                .on_msg_state = aout_application_on_msg_state,
                .context = self
        });

        // Move somewhere else
        if (AOUT_IS_ERR(aout_client_connect(self->client, 0x7f000001, 42424))) {
                aout_loge("could not connect to foreign host");
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

        aout_client_del(&self->client);
        aout_space_free(self->space);
        aout_debug_draw_del(&self->debug_draw);
        aout_renderer_destroy(self->renderer);
        glfwDestroyWindow(self->window);
        aout_ring_del(&self->predictions);
        free(self);
}

aout_res aout_application_run(
                aout_application* self) {
        assert(self);

        uint64_t  last_time = stm_now();
        float64_t accumulator = 0.0;

        while (aout_application_is_running(self)) {
                glfwPollEvents();

                if (glfwWindowShouldClose(self->window) || self->sigint_raised) {
                        if (self->sigint_raised) { printf("\n"); } // CTRL-C
                        aout_application_stop(self);
                        break;
                }

                float64_t const time_step = self->time_step;
                float64_t const delta_time = stm_sec(stm_laptime(&last_time));

                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
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

        self->tick = aout_tick_increment(self->tick, 1);
        self->state_prev = self->state;

        aout_client_update(self->client);

        //if (!aout_tick_filter_rate(self->tick, 2)) { return; }

        aout_input input = { 0 };
        input.right = glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS;
        input.left = glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS;
        input.up = glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS;
        input.down = glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS;

        // Apply input
        aout_body_apply_input(self->body, &input);
        cpSpaceStep(self->space, delta_time);

        self->state = aout_state_from_body(self->body);

        aout_ring_push(self->predictions, &(aout_prediction) {
                .tick = self->tick,
                .input = input,
                .state = self->state
        });

        // Send input
        if (self->is_connected) {
                aout_application_send_msg_input(self);
        }

        aout_client_flush(self->client);
}

static void aout_application_update(
                aout_application* self,
                float64_t delta_time,
                float64_t alpha) {
        assert(self);
        (void) delta_time;

        aout_debug_draw_clear(self->debug_draw);
        cpSpaceDebugDrawOptions options = aout_debug_draw_default_impl(
                self->debug_draw
        );
        cpSpaceDebugDraw(self->space, &options);

        aout_transform trans = {
                .position = self->state.p,
                .scale = { 1.f, 1.f }
        };

        aout_transform trans_prev = {
                .position = self->state_prev.p,
                .scale = { 1.f, 1.f }
        };

        aout_transform interp = aout_transform_lerp(trans, trans_prev, alpha);

        int width, height;
        glfwGetFramebufferSize(self->window, &width, &height);
        assert(width > 0); assert(height > 0);

        aout_renderer_begin(self->renderer, width, height);

        aout_debug_draw_flush(self->debug_draw);

        aout_renderer_render_mesh(
                self->renderer,
                &self->mesh_server,
                &(aout_transform) {
                        .position = self->state_server.p,
                        .scale = { 1.f, 1.f }
                }
        );

        aout_renderer_render_mesh(
                self->renderer,
                &self->mesh_player,
                &interp
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

        self->state_server = msg->state;

        if (aout_ring_empty(self->predictions)) {
                self->state = msg->state;
                return;
        }

        while (!aout_ring_empty(self->predictions)) {
                aout_prediction const* p = aout_ring_front(self->predictions);
                if (aout_tick_cmp(p->tick, msg->tick) < 0) {
                        aout_ring_pop(self->predictions);
                } else {
                        break;
                }
        }

        aout_prediction const* front = aout_ring_front(self->predictions);
        if (aout_ring_empty(self->predictions)
                        || aout_tick_cmp(front->tick, msg->tick) != 0) {
                aout_loge("prediction %ld missing", msg->tick.value);
                return;
        }

        if (aout_state_close(&front->state, &msg->state)) {
                aout_ring_pop(self->predictions);
                return;
        }

        self->state = msg->state;
        // Reset body to msg->state
        cpBodySetPosition(self->body, cpv(msg->state.p.x, msg->state.p.y));
        cpBodySetVelocity(self->body, cpv(msg->state.v.x, msg->state.v.y));

        for (size_t i = 0; i < aout_ring_end(self->predictions); ++i) {
                aout_prediction* p = aout_ring_at(self->predictions, i);

                //aout_state_apply_input(&self->state, &p->input);
                aout_body_apply_input(self->body, &p->input);
                cpSpaceStep(self->space, self->time_step);
                self->state = aout_state_from_body(self->body);
                p->state = self->state;
        }

        aout_ring_pop(self->predictions);
}

static void aout_application_send_msg_input(
                aout_application* self) {
        assert(self);

        if (aout_ring_size(self->predictions) < AOUT_CL_MSG_INPUT_BUFFER_COUNT) {
                aout_loge("prediction buffer should have at least %d elements",
                                AOUT_CL_MSG_INPUT_BUFFER_COUNT);
                return;
        }

        aout_cl_msg_input msg = { 0 };
        msg.tick = aout_tick_decrement(
                self->tick,
                AOUT_CL_MSG_INPUT_BUFFER_COUNT - 1
        );

        for (size_t i = 0; i < AOUT_CL_MSG_INPUT_BUFFER_COUNT; ++i) {
                aout_prediction const* p = aout_ring_rat(self->predictions, i);
                // Oldest input should be first
                msg.inputs[AOUT_CL_MSG_INPUT_BUFFER_COUNT - 1 - i] = p->input;
        }

        aout_client_send_msg_input(self->client, &msg);
}

static void on_sigint(
                void* context) {
        aout_application* self = context;
        self->sigint_raised = 1;
}
