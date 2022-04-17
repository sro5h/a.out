#ifndef CLIENT_DEBUG_DRAW_H
#define CLIENT_DEBUG_DRAW_H

#include <stdint.h>
#include <chipmunk/chipmunk.h>

typedef struct aout_debug_draw aout_debug_draw;

aout_debug_draw* aout_make_debug_new(
                void);

cpSpaceDebugColor aout_debug_draw_color_for_shape(
                aout_debug_draw* debug_draw,
                cpShape* shape);

void aout_debug_draw_dot(
                aout_debug_draw* debug_draw,
                cpFloat size,
                cpVect pos,
                cpSpaceDebugColor fill);

void aout_debug_draw_circle(
                aout_debug_draw* debug_draw,
                cpVect pos,
                cpFloat angle,
                cpFloat radius,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill);

void aout_debug_draw_segment(
                aout_debug_draw* debug_draw,
                cpVect a,
                cpVect b,
                cpSpaceDebugColor color);

void aout_debug_draw_fat_segment(
                aout_debug_draw* debug_draw,
                cpVect a,
                cpVect b,
                cpFloat radius,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill);

void aout_debug_draw_polygon(
                aout_debug_draw* debug_draw,
                int count,
                const cpVect *verts,
                cpFloat radius,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill);

void aout_debug_draw_bb(
                aout_debug_draw* debug_draw,
                cpBB bb,
                cpSpaceDebugColor color);

void aout_debug_draw_flush(
                aout_debug_draw* debug_draw);

void aout_debug_draw_clear(
                aout_debug_draw* debug_draw);

#endif
