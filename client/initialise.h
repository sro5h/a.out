#ifndef CLIENT_INITIALISE_H
#define CLIENT_INITIALISE_H

#include <common/result.h>

typedef enum aout_res_initialise {
        AOUT_INITIALISE_OK,
        AOUT_INITIALISE_ERR_ENET,
        AOUT_INITIALISE_ERR_GLFW,
} aout_res_initialise;

aout_res aout_initialise(
                void);

void aout_terminate(
                void);

#endif
