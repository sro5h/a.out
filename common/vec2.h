#ifndef COMMON_VEC2_H
#define COMMON_VEC2_H

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

COMMON_API float32_t aout_vec2_length_sq(
                aout_vec2 v);

COMMON_API float32_t aout_vec2_length(
                aout_vec2 v);

COMMON_API aout_vec2 aout_vec2_norm(
                aout_vec2 v);

#endif
