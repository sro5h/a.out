#ifndef COMMON_TRANSFORM_H
#define COMMON_TRANSFORM_H

#include "vector2.h"

typedef struct aout_transform {
        aout_vec2 position;
        float rotation;
        aout_vec2 scale;
} aout_transform;

#endif
