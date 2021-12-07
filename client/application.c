#include "application.h"
#include "player_mesh.h"

#include <common/console.h>
#include <common/log.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <sokol/sokol_time.h>
#include <stdlib.h>

static void aout_application_update_fixed(
                aout_application* app,
                double delta_time);

static void aout_application_update(
                aout_application* app,
                double delta_time);

static void on_sigint(
                void* context);

// TODO: add aout_application_desc
// TODO: Maybe don't protect against malloc failure?
aout_application* aout_application_create(
                //aout_application_desc* desc
                void) {
        aout_application* app = malloc(sizeof(*app));

        if (!app) {
                return NULL;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        app->window = glfwCreateWindow(640, 480, "client", NULL, NULL);

        if (!app->window) {
                aout_loge("could not create window");
                goto error_window;
        }

        glfwSetWindowPos(app->window, 0, 0);
        glfwMakeContextCurrent(app->window);
        glfwSwapInterval(1);

        app->renderer = aout_renderer_create();

        if (!app->renderer) {
                aout_loge("could not create renderer");
                goto error_renderer;
        }

        app->client = aout_client_create((aout_client_adapter) { 0 });

        if (!app->client) {
                aout_loge("could not create client");
                goto error_client;
        }

        app->is_running = true;
        app->time_step = 1.0 / 64;
        app->sigint_raised = 0;
        app->player_mesh = aout_player_mesh_create();

        // Move somewhere else
        if (AOUT_IS_ERR(aout_client_connect(app->client, 0x7f000001, 42424))) {
                aout_loge("could not connect to foreign host");
                goto error_connect;
        }

        aout_res res = aout_on_sigint((aout_sig_handler) {
                .callback = on_sigint,
                .context = app
        });

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not set SIGINT handler");
                goto error_signal;
        }

        return app;

// TODO: Zero initialise app (especially pointers) and replace labels with
// if (app->ptr) { destroy(app->ptr) }
error_signal:
error_connect:
        aout_client_destroy(app->client);

error_client:
        aout_renderer_destroy(app->renderer);

error_renderer:
        glfwDestroyWindow(app->window);

error_window:
        free(app);
        return NULL;
}

void aout_application_destroy(
                aout_application* app) {
        if (!app) {
                return;
        }

        assert(app->window); assert(app->client);

        aout_client_destroy(app->client);
        aout_renderer_destroy(app->renderer);
        glfwDestroyWindow(app->window);
        free(app);
}

aout_res aout_application_run(
                aout_application* app) {
        assert(app);

        uint64_t last_time = stm_now();
        double accumulator = 0.0;

        while (aout_application_is_running(app)) {
                if (glfwWindowShouldClose(app->window) || app->sigint_raised) {
                        if (app->sigint_raised) { printf("\n"); } // CTRL-C
                        aout_application_stop(app);
                        // TODO: Maybe move check to end of loop
                        break;
                }

                const uint64_t now = stm_now();
                const double delta_time = stm_sec(stm_diff(now, last_time));
                last_time = now;

                const double time_step = app->time_step;
                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
                        glfwPollEvents();
                        aout_application_update_fixed(app, time_step);
                }

                aout_application_update(app, delta_time);
        }

        return AOUT_OK;
}

void aout_application_stop(
                aout_application* app) {
        assert(app);
        app->is_running = false;
}

bool aout_application_is_running(
                aout_application* app) {
        assert(app);
        return app->is_running;
}

static void aout_application_update_fixed(
                aout_application* app,
                double delta_time) {
        assert(app);
        (void) delta_time;
        aout_client_update(app->client);
}

static void aout_application_update(
                aout_application* app,
                double delta_time) {
        assert(app);
        (void) delta_time;

        int width, height;
        glfwGetFramebufferSize(app->window, &width, &height);
        assert(width > 0); assert(height > 0);

        aout_renderer_begin(app->renderer, width, height);
        aout_renderer_render_mesh(app->renderer, &app->player_mesh);
        aout_renderer_end(app->renderer);

        glfwSwapBuffers(app->window);
}

static void on_sigint(
                void* context) {
        aout_application* app = (aout_application*) context;
        app->sigint_raised = 1;
}
