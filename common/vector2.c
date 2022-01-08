#include "vector2.h"

#include <assert.h>

aout_vec2 aout_vec2_add(
                aout_vec2 a,
                aout_vec2 b) {
        return (aout_vec2) {
                .x = a.x + b.x,
                .y = a.y + b.y
        };
}

aout_vec2 aout_vec2_sub(
                aout_vec2 a,
                aout_vec2 b) {
        return (aout_vec2) {
                .x = a.x - b.x,
                .y = a.y - b.y
        };
}

aout_vec2 aout_vec2_mul(
                aout_vec2 v,
                float32_t f) {
        return (aout_vec2) {
                .x = v.x * f,
                .y = v.y * f
        };
}

aout_vec2 aout_vec2_div(
                aout_vec2 v,
                float32_t f) {
        assert(f != 0.0f);
        return (aout_vec2) {
                .x = v.x / f,
                .y = v.y / f
        };
}

#define AOUT_VEC2_EPSILON (1.0e-6)

COMMON_API bool aout_vec2_eql(
                aout_vec2 const* a,
                aout_vec2 const* b) {
        assert(a); assert(b);

        return aout_f32_eql(a->x, b->x) && aout_f32_eql(a->y, b->y);
}
