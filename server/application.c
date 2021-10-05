#include "application.h"

#include <common/console.h>

#include <chipmunk/chipmunk.h>
#include <sokol/sokol_time.h>
#include <stdio.h>

static void aout_application_update_fixed(
                aout_application* app,
                double delta_time);

static void aout_application_update(
                aout_application* app,
                double delta_time);

static void on_sigint(
                void* context);

aout_application* aout_application_create(
                void) {
        aout_application* app = malloc(sizeof(*app));

        if (!app) {
                return NULL;
        }

        app->space = cpSpaceNew();

        if (!app->space) {
                printf("error: could not create space\n");
                goto error_space;
        }

        cpSpaceSetIterations(app->space, 10);
        cpSpaceSetSleepTimeThreshold(app->space, 0.5f);
        // TODO: Create physics scene

        app->server = aout_server_create();

        if (!app->server) {
                printf("error: could not create server\n");
                goto error_server;
        }

        app->is_running = true;
        app->time_step = 1.0 / 64;
        app->sigint_raised = 0;

        aout_res res = aout_on_sigint((aout_sig_handler) {
                .callback = on_sigint,
                .context = app
        });

        if (AOUT_IS_ERR(res)) {
                printf("error: could not set SIGINT handler\n");
                goto error_signal;
        }

        return app;

error_signal:
        aout_server_destroy(app->server);

error_server:

error_space:
        free(app);
        return NULL;
}

void aout_application_destroy(
                aout_application* app) {
        if (!app) {
                return;
        }

        assert(app->space); assert(app->server);

        aout_server_destroy(app->server);
        //aout_util_space_free_children(app->space); // TODO: Add
        cpSpaceFree(app->space);
        free(app);
}

aout_res aout_application_run(
                aout_application* app) {
        assert(app);

        uint64_t last_time = stm_now();
        double accumulator = 0.0;

        while (aout_application_is_running(app)) {
                if (app->sigint_raised) {
                        aout_application_stop(app);
                }

                const uint64_t now = stm_now();
                const double delta_time = stm_sec(stm_diff(now, last_time));
                last_time = now;

                const double time_step = app->time_step;
                for (accumulator += delta_time; accumulator > time_step;
                                accumulator -= time_step) {
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

        aout_server_update(app->server);
        cpSpaceStep(app->space, delta_time);
}

static void aout_application_update(
                aout_application* app,
                double delta_time) {
        assert(app);
        (void) delta_time;
}

static void on_sigint(
                void* context) {
        aout_application* app = (aout_application*) context;
        app->sigint_raised = 1;
}
