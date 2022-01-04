#ifndef COMMON_VECTOR2_H
#define COMMON_VECTOR2_H

#include "types.h"

typedef struct aout_vec2 {
        float32_t x, y;
} aout_vec2;

aout_vec2 aout_vec2_add(
                aout_vec2 a,
                aout_vec2 b);

aout_vec2 aout_vec2_sub(
                aout_vec2 a,
                aout_vec2 b);

aout_vec2 aout_vec2_mul(
                aout_vec2 v,
                float32_t f);

aout_vec2 aout_vec2_div(
                aout_vec2 v,
                float32_t f);

#endif
