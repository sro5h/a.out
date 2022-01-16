#include "application.h"
#include "player_mesh.h"

#include <common/console.h>
#include <common/log.h>

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

        self->is_running = true;
        self->is_connected = false;
        self->time_step = 1.0 / 32;
        self->sigint_raised = 0;

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
        self->player_mesh = aout_player_mesh_create();

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

        aout_client_destroy(self->client);
        aout_renderer_destroy(self->renderer);
        glfwDestroyWindow(self->window);
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

        glfwPollEvents();

        //if (!aout_tick_filter_rate(&self->tick, 2)) { return; }

        // Send input
        if (self->is_connected) {
                aout_cl_msg_input msg = { 0 };
                msg.up = glfwGetKey(self->window, GLFW_KEY_W) == GLFW_PRESS;
                msg.down = glfwGetKey(self->window, GLFW_KEY_S) == GLFW_PRESS;
                msg.left = glfwGetKey(self->window, GLFW_KEY_A) == GLFW_PRESS;
                msg.right = glfwGetKey(self->window, GLFW_KEY_D) == GLFW_PRESS;

                aout_client_send_msg_input(self->client, &msg);
        }

        aout_client_update(self->client);
}

static void aout_application_update(
                aout_application* self,
                float64_t delta_time,
                float64_t alpha) {
        assert(self);
        (void) delta_time;

        aout_transform interpolated = { 0 };
        interpolated = aout_transform_add(
                aout_transform_mul(self->player_transform, alpha),
                aout_transform_mul(self->player_transform_prev, 1.0 - alpha)
        );

        int width, height;
        glfwGetFramebufferSize(self->window, &width, &height);
        assert(width > 0); assert(height > 0);

        aout_renderer_begin(self->renderer, width, height);

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

        self->player_transform_prev = self->player_transform;
        self->player_transform.position = msg->position;
}

static void on_sigint(
                void* context) {
        aout_application* self = context;
        self->sigint_raised = 1;
}
