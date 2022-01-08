#ifndef COMMON_STATE_H
#define COMMON_STATE_H

#include "vector2.h"

typedef struct cpBody cpBody;

typedef struct aout_state {
        aout_vec2 p;
        aout_vec2 v;
        float32_t r;
} aout_state;

typedef struct aout_state_full {
        union {
                aout_state state;
                struct {
                        aout_vec2 p;
                        aout_vec2 v;
                        float32_t r;
                };
        };

        aout_vec2 f;
} aout_state_full;

COMMON_API aout_state_full aout_state_full_from_body(
                cpBody* body);

COMMON_API bool aout_state_eql(
                aout_state const* a,
                aout_state const* b);

COMMON_API bool aout_state_full_eql(
                aout_state_full const* a,
                aout_state_full const* b);

#endif
