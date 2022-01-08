#include "state.h"
#include "types.h"

#include <chipmunk/chipmunk.h>

#include <assert.h>

aout_state_full aout_state_full_from_body(
                cpBody* body) {
        assert(body);

        aout_state_full state = { 0 };
        state.p.x = cpBodyGetPosition(body).x;
        state.p.y = cpBodyGetPosition(body).y;
        state.v.x = cpBodyGetVelocity(body).x;
        state.v.y = cpBodyGetVelocity(body).y;
        // TODO: Is that correct?
        //state.r   = cpBodyGetAngle(body);
        state.f.x = cpBodyGetForce(body).x;
        state.f.y = cpBodyGetForce(body).y;

        return state;
}

bool aout_state_eql(
                aout_state const* a,
                aout_state const* b) {
        assert(a); assert(b);

        return aout_vec2_eql(&a->p, &b->p) && aout_vec2_eql(&a->v, &b->v)
               && aout_f32_eql(a->r, b->r);
}

bool aout_state_full_eql(
                aout_state_full const* a,
                aout_state_full const* b) {
        assert(a); assert(b);

        return aout_state_eql(&a->state, &b->state) && aout_vec2_eql(&a->f, &b->f);
}
