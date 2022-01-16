#include "vec2.h"

#include <assert.h>
#include <math.h>

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

COMMON_API float32_t aout_vec2_length_sq(
                aout_vec2 v) {
        return (v.x * v.x) + (v.y * v.y);
}

COMMON_API float32_t aout_vec2_length(
                aout_vec2 v) {
        return sqrt(aout_vec2_length_sq(v));
}

COMMON_API aout_vec2 aout_vec2_norm(
                aout_vec2 v) {
        float32_t length = aout_vec2_length(v);
        if (length == 0.0f) {
                return v;
        } else {
                return aout_vec2_div(v, length);
        }
}
