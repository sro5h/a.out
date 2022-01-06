#ifndef CLIENT_PREDICTION_H
#define CLIENT_PREDICTION_H

#include <common/input.h>
#include <common/tick.h>
#include <common/transform.h>

typedef struct aout_prediction {
        aout_tick tick;
        aout_input input;
        aout_transform state;
} aout_prediction;

#endif
