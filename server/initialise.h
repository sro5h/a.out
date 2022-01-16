#ifndef SERVER_INITIALISE_H
#define SERVER_INITIALISE_H

#include <common/result.h>

typedef enum aout_res_initialise {
        AOUT_INITIALISE_OK,
        AOUT_INITIALISE_ERR_ENET,
} aout_res_initialise;

aout_res aout_initialise(
                void);

void aout_terminate(
                void);

#endif
