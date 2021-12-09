#include "application.h"

#include <common/console.h>
#include <common/log.h>
#include <common/util.h>

#include <chipmunk/chipmunk.h>
#include <sokol/sokol_time.h>
#include <stdio.h>
#include <string.h>

#define CLIENT_BODY_MASS   10
#define CLIENT_BODY_RADIUS 10

static void aout_application_update_fixed(
                aout_application* app,
                double delta_time);

static void aout_application_update(
                aout_application* app,
                double delta_time);

static void aout_application_on_connection(
                aout_server* server,
                aout_connection* connection,
                void* context);

static void aout_application_on_disconnection(
                aout_server* server,
                aout_connection* connection,
                void* context);

static void on_sigint(
                void* context);

aout_application* aout_application_create(
                void) {
        // Bodies must be zero
        aout_application* app = calloc(1, sizeof(*app));

        if (!app) {
                return NULL;
        }

        app->is_running = true;
        app->time_step = 1.0 / 64;
        app->sigint_raised = 0;

        app->space = cpSpaceNew();

        if (!app->space) {
                aout_loge("could not create space");
                goto error;
        }

        cpSpaceSetIterations(app->space, 10);
        cpSpaceSetSleepTimeThreshold(app->space, 0.5f);
        // TODO: Create physics scene

        app->server = aout_server_create((aout_server_adapter) {
                .on_connection = aout_application_on_connection,
                .on_disconnection = aout_application_on_disconnection,
                .context = app
        });

        if (!app->server) {
                aout_loge("could not create server");
                goto error;
        }

        aout_res res = aout_on_sigint((aout_sig_handler) {
                .callback = on_sigint,
                .context = app
        });

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not set SIGINT handler");
                goto error;
        }

        return app;

error:
        aout_application_destroy(app);
        return NULL;
}

void aout_application_destroy(
                aout_application* app) {
        if (!app) {
                return;
        }

        aout_server_destroy(app->server);
        aout_space_free(app->space);
        free(app);
}

aout_res aout_application_run(
                aout_application* app) {
        assert(app);

        uint64_t last_time = stm_now();
        double accumulator = 0.0;

        while (aout_application_is_running(app)) {
                if (app->sigint_raised) {
                        printf("\n"); // CTRL-C
                        aout_application_stop(app);
                        // TODO: Could also move this check to the end of loop
                        break;
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

        // Send current state to clients
        // TODO: Send state of all connected clients
        if (app->bodies[0]) {
                aout_sv_msg_state msg = { 0 };
                msg.position.x = cpBodyGetPosition(app->bodies[0]).x;
                msg.position.y = cpBodyGetPosition(app->bodies[0]).y;

                aout_server_send_msg_state(app->server, 0, &msg);
        }

        // TODO: Send immediately
        // aout_server_flush(app->server);
}

static void aout_application_update(
                aout_application* app,
                double delta_time) {
        assert(app);
        (void) delta_time;
}

static void aout_application_on_connection(
                aout_server* server,
                aout_connection* connection,
                void* context) {
        assert(server); assert(connection); assert(context);
        aout_application* app = context;

        assert(!app->bodies[connection->peer_id]);

        cpBody* body = cpSpaceAddBody(app->space, cpBodyNew(
                CLIENT_BODY_MASS,
                cpMomentForCircle(
                        CLIENT_BODY_MASS,
                        0,
                        CLIENT_BODY_RADIUS,
                        cpvzero
                )
        ));

        cpBodySetPosition(body, cpvzero);

        cpShape* shape = cpSpaceAddShape(app->space, cpCircleShapeNew(
                body,
                CLIENT_BODY_RADIUS,
                cpvzero
        ));
        cpShapeSetElasticity(shape, 0.0f);
        cpShapeSetFriction(shape, 0.7f);

        app->bodies[connection->peer_id] = body;
}

static void aout_application_on_disconnection(
                aout_server* server,
                aout_connection* connection,
                void* context) {
        assert(server); assert(connection); assert(context);
        aout_application* app = context;

        cpBody* body = app->bodies[connection->peer_id];
        assert(body);

        // Don't use post step callbacks as this will never be called inside
        // cpSpaceStep.
        aout_body_free(body);
        app->bodies[connection->peer_id] = NULL;
}

static void on_sigint(
                void* context) {
        aout_application* app = (aout_application*) context;
        app->sigint_raised = 1;
}
