#ifndef CLIENT_APPLICATION_H
#define CLIENT_APPLICATION_H

#include "client.h"
#include "renderer.h"

#include <common/tick.h>

#include <signal.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct aout_application {
        // Window and graphics
        GLFWwindow* window;
        aout_renderer* renderer;
        aout_mesh player_mesh;
        bool is_running;
        // Networking
        aout_client* client;
        bool is_connected;
        // Timing
        double time_step; // Maybe use ticks_per_second instead
        aout_tick tick;
        // Other
        sig_atomic_t sigint_raised;
        aout_transform player_transform;
} aout_application;

typedef enum aout_application_res {
        AOUT_APPLICATION_OK,
        AOUT_APPLICATION_ERR
} aout_application_res;

aout_application* aout_application_create(
                void);

void aout_application_destroy(
                aout_application* self);

void aout_application_init(
                aout_application* self);

aout_res aout_application_run(
                aout_application* self);

void aout_application_stop(
                aout_application* self);

bool aout_application_is_running(
                aout_application* self);

#endif
