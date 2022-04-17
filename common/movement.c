#include "movement.h"

#include <chipmunk/chipmunk.h>

#define AOUT_PLAYER_FORCE (5000)
#define AOUT_PLAYER_FRICTION (0.7)
#define AOUT_PLAYER_VELOCITY_MAX (250.0)

void aout_body_velocity_update(
                cpBody* body,
                cpVect gravity,
                cpFloat damping,
                cpFloat dt) {
        damping *= AOUT_PLAYER_FRICTION;
        cpBodyUpdateVelocity(body, gravity, damping, dt);
        cpBodySetVelocity(body, cpvclamp(
                cpBodyGetVelocity(body), AOUT_PLAYER_VELOCITY_MAX
        ));
}

void aout_body_apply_input(
                cpBody* body,
                aout_input* input) {
        assert(body); assert(input);

        cpVect direction = { 0 };

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

        cpVect force = cpvmult(cpvnormalize(direction), AOUT_PLAYER_FORCE);
        cpBodySetForce(body, force);
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
