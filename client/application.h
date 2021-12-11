#ifndef CLIENT_APPLICATION_H
#define CLIENT_APPLICATION_H

#include "client.h"
#include "renderer.h"

#include <signal.h>

typedef struct GLFWwindow GLFWwindow;

typedef struct aout_application {
        aout_client* client;
        GLFWwindow* window;
        aout_renderer* renderer;
        bool is_running;
        bool is_connected;
        double time_step; // Maybe use ticks_per_second instead
        sig_atomic_t sigint_raised;
        aout_mesh player_mesh;
        aout_transform player_transform;
} aout_application;

typedef enum aout_application_res {
        AOUT_APPLICATION_OK,
        AOUT_APPLICATION_ERR
} aout_application_res;

aout_application* aout_application_create(
                void);

void aout_application_destroy(
                aout_application* application);

void aout_application_init(
                aout_application* application);

aout_res aout_application_run(
                aout_application* application);

void aout_application_stop(
                aout_application* application);

bool aout_application_is_running(
                aout_application* application);

#endif
