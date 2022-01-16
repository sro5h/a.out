#include "player_mesh.h"
#include "vertex.h"

#include <cglm/mat4.h>
#include <sokol/sokol_gfx.h>

static char const* vs_source =
        "#version 330\n"
        "layout(location=0) in vec2 position;\n"
        "layout(location=1) in vec2 uv;\n"
        "layout(location=2) in float radius;\n"
        "layout(location=3) in vec4 color;\n"
        "uniform mat4 mvp;\n"
        "out struct {\n"
        "       vec2 uv;\n"
        "       vec4 color;\n"
        "} frag;\n"
        "void main() {\n"
        "       gl_Position = mvp * vec4(position + radius * uv, 0, 1);\n"
        "       frag.uv = uv;\n"
        "       frag.color = color;\n"
        "}\n";

static char const* fs_source =
        "#version 330\n"
        "in struct {"
        "       vec2 uv;\n"
        "       vec4 color;\n"
        "} frag;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "       float len  = length(frag.uv);\n"
        "       float fw   = length(fwidth(frag.uv));\n"
        "       float mask = smoothstep(-1, fw - 1, -len);\n"
        "       float outline      = 1 - fw;\n"
        "       float outline_mask = smoothstep(outline - fw, outline, len);\n"
        "       color = frag.color * outline_mask * mask;\n"
        "}\n";

aout_mesh aout_player_mesh_create(
                aout_rgba8 color) {
        aout_vertex const vertices[] = {
                {
                        .position = { 0.f, 0.f },
                        .uv = { -1, -1 },
                        .radius = 50.f,
                        .color = color
                },
                {
                        .position = { 0.f, 0.f },
                        .uv = { -1,  1 },
                        .radius = 50.f,
                        .color = color
                },
                {
                        .position = { 0.f, 0.f },
                        .uv = {  1,  1 },
                        .radius = 50.f,
                        .color = color
                },
                {
                        .position = { 0.f, 0.f },
                        .uv = {  1, -1 },
                        .radius = 50.f,
                        .color = color
                }
        };

        aout_index const indices[] = { 0, 1, 2, 0, 2, 3 };
        size_t const index_count = 6;

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
                .index_count = index_count,
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
                        .colors[0].blend = {
                                .enabled = true,
                                .src_factor_rgb = SG_BLENDFACTOR_ONE,
                                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA
                        },
                        .index_type = SG_INDEXTYPE_UINT16,
                        .layout = layout
                })
        };
}
