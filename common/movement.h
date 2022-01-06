#ifndef COMMON_MOVEMENT_H
#define COMMON_MOVEMENT_H

#include "input.h"

typedef struct cpBody cpBody;

COMMON_API void aout_apply_movement(
                cpBody* body,
                aout_input const* input);

#endif
