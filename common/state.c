#include "state.h"

#include <math.h>

#define AOUT_STATE_F32_EPSILON (0.001f)

static bool aout_f32_close(
                float32_t a,
                float32_t b);

bool aout_state_close(
                aout_state const* a,
                aout_state const* b) {
        return aout_f32_close(a->p.x, b->p.x) && aout_f32_close(a->p.y, b->p.y)
            && aout_f32_close(a->v.x, b->v.x) && aout_f32_close(a->v.y, b->v.y);
}

void aout_state_apply_input(
                aout_state* self,
                aout_input const* input) {
        assert(self); assert(input);

        aout_vec2 direction = { 0 };

        if (input->right) {
                direction.x += 1;
        }

        if (input->left) {
                direction.x -= 1;
        }

        if (input->up) {
                direction.y += 1;
        }

        if (input->down) {
                direction.y -= 1;
        }

        aout_vec2 velocity = aout_vec2_mul(aout_vec2_norm(direction), 25);
        self->p = aout_vec2_add(self->p, velocity);
        self->v = velocity;
}

bool aout_f32_close(
                float32_t a,
                float32_t b) {
        return fabs(a - b) < AOUT_STATE_F32_EPSILON;
}
