#ifndef COMMON_STATE_H
#define COMMON_STATE_H

#include "input.h"
#include "vec2.h"

typedef struct aout_state {
        aout_vec2 p;
        aout_vec2 v;
} aout_state;

COMMON_API bool aout_state_close(
                aout_state const* a,
                aout_state const* b);

COMMON_API void aout_state_apply_input(
                aout_state* self,
                aout_input const* input);

#endif
