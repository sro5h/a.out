#include "debug_draw_glue.h"

static void aout_draw_dot(
                cpFloat size,
                cpVect pos,
                cpSpaceDebugColor color,
                cpDataPointer debug_draw) {
        aout_debug_draw_dot((aout_debug_draw*) debug_draw, size, pos, color);
}

static void aout_draw_circle(
                cpVect p,
                cpFloat a,
                cpFloat r,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill,
                cpDataPointer debug_draw) {
        aout_debug_draw_circle(
                (aout_debug_draw*) debug_draw, p, a, r,  outline, fill
        );
}

static void aout_draw_segment(
                cpVect a,
                cpVect b,
                cpSpaceDebugColor color,
                cpDataPointer debug_draw) {
        aout_debug_draw_segment((aout_debug_draw*) debug_draw, a, b, color);
}

static void aout_draw_fat_segment(
                cpVect a,
                cpVect b,
                cpFloat r,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill,
                cpDataPointer debug_draw) {
        aout_debug_draw_fat_segment(
                (aout_debug_draw*) debug_draw, a, b, r, outline, fill
        );
}

static void aout_draw_polygon(
                int count,
                const cpVect* verts,
                cpFloat r,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill,
                cpDataPointer debug_draw) {
        aout_debug_draw_polygon(
                (aout_debug_draw*) debug_draw, count, verts, r, outline, fill
        );
}

static cpSpaceDebugColor aout_color_for_shape(
                cpShape* shape,
                cpDataPointer debug_draw) {
        return aout_debug_draw_color_for_shape(
                (aout_debug_draw*) debug_draw, shape
        );
}

cpSpaceDebugDrawOptions aout_debug_draw_default_impl(
                aout_debug_draw* self) {
        return (cpSpaceDebugDrawOptions) {
                .drawDot = aout_draw_dot,
                .drawCircle = aout_draw_circle,
                .drawSegment = aout_draw_segment,
                .drawFatSegment = aout_draw_fat_segment,
                .drawPolygon = aout_draw_polygon,

                .flags = CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_CONSTRAINTS
                        | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS,

                .colorForShape = aout_color_for_shape,
                .shapeOutlineColor =   { 0x33 / 255.0f, 0x33 / 255.0f, 0x33 / 255.0f, 1.0f },
                .constraintColor =     { 0.0f, 0.75f, 0.0f, 1.0f },
                .collisionPointColor = { 0.0f, 0.75f, 0.75f, 1.0f },

                .data = self,
        };
}
