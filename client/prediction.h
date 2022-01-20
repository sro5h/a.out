#ifndef CLIENT_PREDICTION_H
#define CLIENT_PREDICTION_H

#include <common/input.h>
#include <common/state.h>
#include <common/tick.h>

typedef struct aout_prediction {
        aout_tick tick;
        aout_input input;
        aout_state state;
} aout_prediction;

#endif
