#ifndef COMMON_VECTOR2_H
#define COMMON_VECTOR2_H

#include "types.h"

typedef struct aout_vec2 {
        float32_t x, y;
} aout_vec2;

COMMON_API aout_vec2 aout_vec2_add(
                aout_vec2 a,
                aout_vec2 b);

COMMON_API aout_vec2 aout_vec2_sub(
                aout_vec2 a,
                aout_vec2 b);

COMMON_API aout_vec2 aout_vec2_mul(
                aout_vec2 v,
                float32_t f);

COMMON_API aout_vec2 aout_vec2_div(
                aout_vec2 v,
                float32_t f);

COMMON_API bool aout_vec2_eql(
                aout_vec2 const* a,
                aout_vec2 const* b);

#endif
