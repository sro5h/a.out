#ifndef CLIENT_INITIALISE_H
#define CLIENT_INITIALISE_H

#include <common/result.h>

typedef enum aout_initialise_res {
        AOUT_INITIALISE_OK,
        AOUT_INITIALISE_ERR_ENET,
        AOUT_INITIALISE_ERR_GLFW,
} aout_initialise_res;

aout_res aout_initialise(
                void);

void aout_terminate(
                void);

#endif
