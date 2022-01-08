#ifndef COMMON_MOVEMENT_H
#define COMMON_MOVEMENT_H

#include "input.h"

typedef struct cpBody cpBody;

COMMON_API void aout_movement_apply(
                cpBody* body,
                aout_input const* input);

#endif
