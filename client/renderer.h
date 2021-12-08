#ifndef CLIENT_RENDERER_H
#define CLIENT_RENDERER_H

#include "mesh.h"

#include <common/transform.h>

typedef struct aout_renderer aout_renderer;

aout_renderer* aout_renderer_create(
                void);

void aout_renderer_destroy(
                aout_renderer* renderer);

void aout_renderer_set_view(
                aout_renderer* renderer,
                size_t width,
                size_t height);

void aout_renderer_begin(
                aout_renderer* renderer,
                size_t width,
                size_t height);

void aout_renderer_end(
                aout_renderer* renderer);

void aout_renderer_render_mesh(
                aout_renderer* renderer,
                aout_mesh const* mesh,
                aout_transform const* transform);

#endif
