#ifndef CLIENT_RENDERER_H
#define CLIENT_RENDERER_H

#include "mesh.h"

typedef struct aout_renderer aout_renderer;

aout_renderer* aout_renderer_create(
                void);

void aout_renderer_destroy(
                aout_renderer* renderer);

void aout_renderer_begin(
                aout_renderer* renderer,
                size_t width,
                size_t height);

void aout_renderer_end(
                aout_renderer* renderer);

void aout_renderer_render_mesh(
                aout_renderer* renderer,
                aout_mesh* mesh/*,
                transform*/);

#endif
