#ifndef COMMON_TRANSFORM_H
#define COMMON_TRANSFORM_H

#include "vec2.h"

typedef struct aout_transform {
        aout_vec2 position;
        float32_t rotation;
        aout_vec2 scale;
} aout_transform;

COMMON_API aout_transform aout_transform_add(
                aout_transform a,
                aout_transform b);

COMMON_API aout_transform aout_transform_sub(
                aout_transform a,
                aout_transform b);

COMMON_API aout_transform aout_transform_mul(
                aout_transform t,
                float32_t f);

COMMON_API aout_transform aout_transform_div(
                aout_transform t,
                float32_t f);

#endif
