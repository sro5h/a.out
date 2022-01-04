#include "transform.h"

#include <assert.h>

aout_transform aout_transform_add(
                aout_transform a,
                aout_transform b) {
        return (aout_transform) {
                .position = aout_vec2_add(a.position, b.position),
                // TODO: Maybe clamp rotation to [0,2pi]
                .rotation = a.rotation + b.rotation,
                .scale = aout_vec2_add(a.scale, b.scale)
        };
}

aout_transform aout_transform_sub(
                aout_transform a,
                aout_transform b) {
        return (aout_transform) {
                .position = aout_vec2_sub(a.position, b.position),
                // TODO: Maybe clamp rotation to [0,2pi]
                .rotation = a.rotation - b.rotation,
                .scale = aout_vec2_sub(a.scale, b.scale)
        };
}

aout_transform aout_transform_mul(
                aout_transform t,
                float32_t f) {
        return (aout_transform) {
                .position = aout_vec2_mul(t.position, f),
                .rotation = t.rotation * f,
                .scale = aout_vec2_mul(t.scale, f)
        };
}

aout_transform aout_transform_div(
                aout_transform t,
                float32_t f) {
        assert(f != 0.0f);
        return (aout_transform) {
                .position = aout_vec2_div(t.position, f),
                .rotation = t.rotation / f,
                .scale = aout_vec2_div(t.scale, f)
        };
}
