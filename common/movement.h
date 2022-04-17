#ifndef COMMON_MOVEMENT_H
#define COMMON_MOVEMENT_H

#include "input.h"
#include "state.h"

// TODO: Remove
#include <chipmunk/chipmunk.h>

typedef struct cpBody cpBody;

COMMON_API void aout_body_velocity_update(
                cpBody* body,
                cpVect gravity,
                cpFloat damping,
                cpFloat dt);

COMMON_API void aout_body_apply_input(
                cpBody* body,
                aout_input* input);

COMMON_API aout_state aout_state_from_body(
                cpBody* body);

#endif
