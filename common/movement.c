#include "movement.h"

#include <chipmunk/chipmunk.h>

COMMON_API void aout_movement_apply(
                cpBody* body,
                aout_input const* input) {
        cpVect direction = cpvzero;

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

        cpVect velocity = cpvzero;

        if (cpvlengthsq(direction) != 0.0) {
                velocity = cpvmult(cpvnormalize(direction), 250);
        }

        cpBodySetVelocity(body, velocity);
}
