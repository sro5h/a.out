#include "player_mesh.h"
#include "vertex.h"

#include <cglm/mat4.h>
#include <sokol/sokol_gfx.h>

static char const* vs_source =
        "#version 330\n"
        "layout(location=0) in vec2 position;\n"
        "layout(location=1) in vec2 uv;\n"
        "layout(location=2) in float radius;\n"
        "layout(location=3) in vec4 color0;\n"
        "uniform mat4 mvp;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "       gl_Position = mvp * vec4(position, 0, 1);\n"
        "       color = color0;\n"
        "}\n";

static char const* fs_source =
        "#version 330\n"
        "in vec4 color;\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "       frag_color = color;\n"
        "}\n";

aout_mesh aout_player_mesh_create(
                void) {
        aout_rgba8 color = { 0xf6, 0x08, 0x1e, 0xff };
        aout_vertex const vertices[] = {
                { .position = {   0.f,  50.f }, .color = color },
                { .position = {  50.f, -50.f }, .color = color },
                { .position = { -50.f, -50.f }, .color = color }
        };

        aout_index const indices[] = { 0, 1, 2 };

        sg_layout_desc layout = {
                .attrs[0] = {
                        .format = SG_VERTEXFORMAT_FLOAT2,
                        .offset = offsetof(aout_vertex, position)
                },
                .attrs[1] = {
                        .format = SG_VERTEXFORMAT_FLOAT2,
                        .offset = offsetof(aout_vertex, uv)
                },
                .attrs[2] = {
                        .format = SG_VERTEXFORMAT_FLOAT,
                        .offset = offsetof(aout_vertex, radius)
                },
                .attrs[3] = {
                        .format = SG_VERTEXFORMAT_UBYTE4N,
                        .offset = offsetof(aout_vertex, color)
                }
        };

        return (aout_mesh) {
                .vertex_buffer = sg_make_buffer(&(sg_buffer_desc) {
                        .type = SG_BUFFERTYPE_VERTEXBUFFER,
                        .usage = SG_USAGE_IMMUTABLE,
                        .data = SG_RANGE(vertices),
                }),
                .index_buffer = sg_make_buffer(&(sg_buffer_desc) {
                        .type = SG_BUFFERTYPE_INDEXBUFFER,
                        .usage = SG_USAGE_IMMUTABLE,
                        .data = SG_RANGE(indices),
                }),
                .index_count = 3,
                .pipeline = sg_make_pipeline(&(sg_pipeline_desc) {
                        .shader = sg_make_shader(&(sg_shader_desc) {
                                .vs.uniform_blocks[0] = {
                                        .size = sizeof(mat4),
                                        .uniforms[0] = {
                                                .name = "mvp",
                                                .type = SG_UNIFORMTYPE_MAT4,
                                        },
                                },
                                .vs.source = vs_source,
                                .fs.source = fs_source
                        }),
                        .index_type = SG_INDEXTYPE_UINT16,
                        .layout = layout
                })
        };
}
