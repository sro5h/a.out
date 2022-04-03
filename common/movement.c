#include "movement.h"

#include <chipmunk/chipmunk.h>

void aout_body_apply_input(
                cpBody* body,
                aout_input* input) {
        assert(body); assert(input);

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

        aout_vec2 velocity = aout_vec2_mul(aout_vec2_norm(direction), 250);
        cpBodySetVelocity(body, cpv(velocity.x, velocity.y));
}

aout_state aout_state_from_body(
                cpBody* body) {
        return (aout_state) {
                .p = {
                        .x = cpBodyGetPosition(body).x,
                        .y = cpBodyGetPosition(body).y,
                },
                .v = {
                        .x = cpBodyGetVelocity(body).x,
                        .y = cpBodyGetVelocity(body).y,
                },
        };
}
