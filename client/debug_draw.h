#ifndef CLIENT_DEBUG_DRAW_H
#define CLIENT_DEBUG_DRAW_H

#include <stdint.h>
#include <chipmunk/chipmunk.h>

typedef struct aout_debug_draw aout_debug_draw;

aout_debug_draw* aout_debug_draw_create(
                void);

void aout_debug_draw_destroy(
                aout_debug_draw* self);

cpSpaceDebugColor aout_debug_draw_color_for_shape(
                aout_debug_draw* self,
                cpShape* shape);

void aout_debug_draw_set_view(
                aout_debug_draw* self,
                size_t width,
                size_t height);

void aout_debug_draw_dot(
                aout_debug_draw* self,
                cpFloat size,
                cpVect pos,
                cpSpaceDebugColor fill);

void aout_debug_draw_circle(
                aout_debug_draw* self,
                cpVect pos,
                cpFloat angle,
                cpFloat radius,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill);

void aout_debug_draw_segment(
                aout_debug_draw* self,
                cpVect a,
                cpVect b,
                cpSpaceDebugColor color);

void aout_debug_draw_fat_segment(
                aout_debug_draw* self,
                cpVect a,
                cpVect b,
                cpFloat radius,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill);

void aout_debug_draw_polygon(
                aout_debug_draw* self,
                int count,
                const cpVect *verts,
                cpFloat radius,
                cpSpaceDebugColor outline,
                cpSpaceDebugColor fill);

void aout_debug_draw_bb(
                aout_debug_draw* self,
                cpBB bb,
                cpSpaceDebugColor color);

void aout_debug_draw_flush(
                aout_debug_draw* self);

void aout_debug_draw_clear(
                aout_debug_draw* self);

#endif
