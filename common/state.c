#include "state.h"

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
