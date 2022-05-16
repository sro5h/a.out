#ifndef CLIENT_RENDERER_H
#define CLIENT_RENDERER_H

#include "mesh.h"

#include <common/transform.h>

typedef struct aout_renderer aout_renderer;

aout_renderer* aout_renderer_new(
                void);

void aout_renderer_del(
                aout_renderer** out_self);

void aout_renderer_set_view(
                aout_renderer* self,
                size_t width,
                size_t height);

void aout_renderer_begin(
                aout_renderer* self,
                size_t width,
                size_t height);

void aout_renderer_end(
                aout_renderer* self);

void aout_renderer_render_mesh(
                aout_renderer* self,
                aout_mesh const* mesh,
                aout_transform const* transform);

#endif
