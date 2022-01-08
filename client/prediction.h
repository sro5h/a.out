#ifndef CLIENT_PREDICTION_H
#define CLIENT_PREDICTION_H

#include <common/input.h>
#include <common/tick.h>
#include <common/state.h>

typedef struct aout_prediction {
        aout_tick tick;
        aout_input input;
        aout_state_full state;
} aout_prediction;

#endif
