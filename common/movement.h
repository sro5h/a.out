#ifndef COMMON_MOVEMENT_H
#define COMMON_MOVEMENT_H

#include "input.h"
#include "state.h"

typedef struct cpBody cpBody;

COMMON_API void aout_body_apply_input(
                cpBody* body,
                aout_input* input);

COMMON_API aout_state aout_state_from_body(
                cpBody* body);

#endif
