#include "renderer.h"

#include <stdlib.h>

struct aout_renderer {
        sg_pass_action pass_action;
};

aout_renderer* aout_renderer_create(
                void) {
        aout_renderer* renderer = malloc(sizeof(*renderer));
        assert(renderer);

        sg_setup(&(sg_desc) { 0 });

        renderer->pass_action = (sg_pass_action) {
                .colors[0] = {
                        .action = SG_ACTION_CLEAR,
                        .value = { 0 }
                }
        };

        return renderer;
}

void aout_renderer_destroy(
                aout_renderer* renderer) {
        if (renderer) {
                sg_shutdown();
                free(renderer);
        }
}

void aout_renderer_begin(
                aout_renderer* renderer,
                size_t width,
                size_t height) {
        sg_begin_default_pass(&renderer->pass_action, width, height);
}

void aout_renderer_end(
                aout_renderer* renderer) {
        (void) renderer;

        sg_end_pass();
        sg_commit();
}

void aout_renderer_render_mesh(
                aout_renderer* renderer,
                aout_mesh* mesh) {
        (void) renderer;

        sg_bindings bindings = {
                .vertex_buffers[0] = mesh->vertex_buffer,
                .index_buffer = mesh->index_buffer
        };

        sg_apply_pipeline(mesh->pipeline);
        sg_apply_bindings(&bindings);
        // sg_apply_uniforms(...);
        sg_draw(0, mesh->index_count, 1);
}
