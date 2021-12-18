#include "renderer.h"

#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <stdlib.h>

struct aout_renderer {
        sg_pass_action pass_action;
        size_t view_width, view_height;
        mat4 view_matrix;
};

aout_renderer* aout_renderer_create(
                void) {
        aout_renderer* self = calloc(1, sizeof(*self));

        if (!self) {
                return NULL;
        }

        sg_setup(&(sg_desc) { 0 });

        self->pass_action = (sg_pass_action) {
                .colors[0] = {
                        .action = SG_ACTION_CLEAR,
                        .value = { 0 }
                }
        };

        aout_renderer_set_view(self, 640, 360);

        return self;
}

void aout_renderer_destroy(
                aout_renderer* self) {
        if (self) {
                sg_shutdown();
                free(self);
        }
}

void aout_renderer_set_view(
                aout_renderer* self,
                size_t width,
                size_t height) {
        assert(self);

        self->view_width = width;
        self->view_height = height;

        glm_mat4_identity(self->view_matrix);
        glm_scale(self->view_matrix, (vec3) {
                1.0f / width,
                1.0f / height,
                1
        });
}

void aout_renderer_begin(
                aout_renderer* self,
                size_t width,
                size_t height) {
        assert(self);
        sg_begin_default_pass(&self->pass_action, width, height);
}

void aout_renderer_end(
                aout_renderer* self) {
        assert(self);
        (void) self;

        sg_end_pass();
        sg_commit();
}

void aout_renderer_render_mesh(
                aout_renderer* self,
                aout_mesh const* mesh,
                aout_transform const* transform) {
        assert(self); assert(mesh); assert(transform);

        sg_bindings bindings = {
                .vertex_buffers[0] = mesh->vertex_buffer,
                .index_buffer = mesh->index_buffer
        };

        mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;
        glm_translate(model_matrix, (vec3) {
                transform->position.x,
                transform->position.y,
                0
        });
        // TODO: Rotation and scale

        mat4 mvp = GLM_MAT4_IDENTITY_INIT;
        glm_mat4_mul(self->view_matrix, model_matrix, mvp);

        sg_apply_pipeline(mesh->pipeline);
        sg_apply_bindings(&bindings);
        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &(sg_range) {
                .ptr = &mvp,
                .size = sizeof(mvp),
        });
        sg_draw(0, mesh->index_count, 1);
}
