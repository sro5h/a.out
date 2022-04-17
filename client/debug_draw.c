#include "debug_draw.h"

#include <sokol/sokol_gfx.h>

#define AOUT_VERTEX_COUNT_MAX (64 * 1024)
#define AOUT_INDEX_COUNT_MAX  (4 * AOUT_VERTEX_COUNT_MAX)

#define GLSL33(x) "#version 330\n" #x

typedef struct priv_float2 {
        float x, y;
} priv_float2;

typedef struct priv_rgba8 {
        uint8_t r, g, b, a;
} priv_rgba8;

typedef struct priv_vertex {
        priv_float2 pos;
        priv_float2 uv;
        float r;
        priv_rgba8 fill;
        priv_rgba8 outline;
} priv_vertex;

typedef uint16_t priv_index;

typedef struct aout_debug_draw {
        cpTransform view_matrix;
        float line_scale;

        sg_buffer vertex_buffer, index_buffer;
        sg_bindings bindings;
        sg_pipeline pipeline;

        priv_vertex vertices[AOUT_VERTEX_COUNT_MAX];
        priv_index indices[AOUT_INDEX_COUNT_MAX];
        size_t vertex_count, index_count;
} aout_debug_draw;

typedef struct priv_view_matrix {
        float entries[16];
} priv_view_matrix;

static
priv_rgba8 cp_to_rgba(
                cpSpaceDebugColor c) {
        return (priv_rgba8) {
                .r = (uint8_t) (0xff * c.r),
                .g = (uint8_t) (0xff * c.g),
                .b = (uint8_t) (0xff * c.b),
                .a = (uint8_t) (0xff * c.a),
        };
}

static
cpSpaceDebugColor rgba_to_cp(
                priv_rgba8 c) {
        return (cpSpaceDebugColor) {
                .r = c.r / 255.f,
                .g = c.g / 255.f,
                .b = c.b / 255.f,
                .a = c.a / 255.f,
        };
}

aout_debug_draw aout_make_debug_draw(
                void) {
        sg_buffer vertex_buffer = sg_make_buffer(&(sg_buffer_desc) {
                .label = "aout_debug_draw vertex buffer",
                .size = AOUT_VERTEX_COUNT_MAX * sizeof(priv_vertex),
                .type = SG_BUFFERTYPE_VERTEXBUFFER,
                .usage = SG_USAGE_STREAM,
        });

        sg_buffer index_buffer = sg_make_buffer(&(sg_buffer_desc) {
                .label = "aout_debug_draw index buffer",
                .size = AOUT_INDEX_COUNT_MAX * sizeof(priv_index),
                .type = SG_BUFFERTYPE_INDEXBUFFER,
                .usage = SG_USAGE_STREAM,
        });

        sg_shader shader = sg_make_shader(&(sg_shader_desc) {
                .vs.uniform_blocks[0] = {
                        .size = sizeof(priv_view_matrix),
                        .uniforms[0] = {
                                .name = "view_matrix",
                                .type = SG_UNIFORMTYPE_MAT4
                        },
                },
                .vs.source = GLSL33(
                        layout(location = 0) in vec2 pos;
                        layout(location = 1) in vec2 uv;
                        layout(location = 2) in float radius;
                        layout(location = 3) in vec4 fill;
                        layout(location = 4) in vec4 outline;

                        uniform mat4 view_matrix;

                        out struct {
                                vec2 uv;
                                vec4 fill;
                                vec4 outline;
                        } frag;

                        void main() {
                                gl_Position = view_matrix * vec4(pos + radius * uv, 0, 1);
                                frag.uv = uv;
                                frag.fill = fill;
                                frag.fill.rgb *= fill.a;
                                frag.outline = outline;
                                frag.outline.a *= outline.a;
                        }
                ),
                .fs.source = GLSL33(
                        in struct {
                                vec2 uv;
                                vec4 fill;
                                vec4 outline;
                        } frag;

                        out vec4 color;

                        void main() {
                                float len = length(frag.uv);
                                float fw = length(fwidth(frag.uv));
                                float mask = smoothstep(-1, fw - 1, -len);

                                float outline = 1 - fw;
                                float outline_mask = smoothstep(outline - fw, outline, len);
                                color = (frag.fill + (frag.outline - frag.fill * frag.outline.a) * outline_mask) * mask;
                        }
                ),
        });

        return (aout_debug_draw) {
                .line_scale = 1.0f,
                .vertex_buffer = vertex_buffer,
                .index_buffer = index_buffer,
                .bindings = (sg_bindings) {
                        .vertex_buffers[0] = vertex_buffer,
                        .index_buffer = index_buffer,
                },
                .pipeline = sg_make_pipeline(&(sg_pipeline_desc) {
                        .shader = shader,
                        .colors[0].blend = {
                                .enabled = true,
                                .src_factor_rgb = SG_BLENDFACTOR_ONE,
                                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA
                        },
                        .index_type = SG_INDEXTYPE_UINT16,
                        .layout = {
                                .attrs[0] = {
                                        .offset = offsetof(priv_vertex, pos),
                                        .format = SG_VERTEXFORMAT_FLOAT2
                                },
                                .attrs[1] = {
                                        .offset = offsetof(priv_vertex, uv),
                                        .format = SG_VERTEXFORMAT_FLOAT2
                                },
                                .attrs[2] = {
                                        .offset = offsetof(priv_vertex, r),
                                        .format = SG_VERTEXFORMAT_FLOAT
                                },
                                .attrs[3] = {
                                        .offset = offsetof(priv_vertex, fill),
                                        .format = SG_VERTEXFORMAT_UBYTE4N
                                },
                                .attrs[4] = {
                                        .offset = offsetof(priv_vertex, outline),
                                        .format = SG_VERTEXFORMAT_UBYTE4N
                                },
                        },
                }),
        };
}

cpSpaceDebugColor aout_debug_draw_color_for_shape(
                aout_debug_draw* debug_draw,
                cpShape* shape) {
        (void) debug_draw;

        if (cpShapeGetSensor(shape)) {
                return rgba_to_cp((priv_rgba8) { 0xff, 0xff, 0xff, 0x1a });
        } else {
                cpBody* body = cpShapeGetBody(shape);

                if (cpBodyIsSleeping(body)) {
                        return rgba_to_cp((priv_rgba8) { 0x58, 0x6e, 0x75, 0xff });
                } else {
                        return rgba_to_cp((priv_rgba8) { 0xf6, 0x08, 0x1e, 0xff });
                }
        }
}

static
priv_vertex* aout_debug_draw_push_vertices(
                aout_debug_draw* debug_draw,
                size_t vcount,
                const priv_index* index_src,
                size_t icount) {
        cpAssertHard(
                debug_draw->vertex_count + vcount <= AOUT_VERTEX_COUNT_MAX
                        && debug_draw->index_count + icount <= AOUT_INDEX_COUNT_MAX,
                "Geometry buffer full."
        );

        priv_vertex* vertex_dst = debug_draw->vertices + debug_draw->vertex_count;
        size_t base = debug_draw->vertex_count;
        debug_draw->vertex_count += vcount;

        priv_index* index_dst = debug_draw->indices + debug_draw->index_count;
        for (size_t i = 0; i < icount; ++i) {
                index_dst[i] = index_src[i] + (priv_index) base;
        }
        debug_draw->index_count += icount;

        return vertex_dst;
}

void aout_debug_draw_dot(
                aout_debug_draw* debug_draw,
                cpFloat size,
                cpVect pos,
                cpSpaceDebugColor _fill) {
        float r = (float) size * 0.5f * debug_draw->line_scale;
        priv_rgba8 fill = cp_to_rgba(_fill);

        priv_vertex* vertices = aout_debug_draw_push_vertices(
                debug_draw,
                4,
                (priv_index[]) { 0, 1, 2, 0, 2, 3 },
                6
        );

        vertices[0] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, { -1, -1 }, r, fill, fill
        };
        vertices[1] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, { -1,  1 }, r, fill, fill
        };
        vertices[2] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, {  1,  1 }, r, fill, fill
        };
        vertices[3] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, {  1, -1 }, r, fill, fill
        };
}

void aout_debug_draw_circle(
                aout_debug_draw* debug_draw,
                cpVect pos,
                cpFloat angle,
                cpFloat radius,
                cpSpaceDebugColor _outline,
                cpSpaceDebugColor _fill) {
        float r = (float) radius + debug_draw->line_scale;
        priv_rgba8 fill = cp_to_rgba(_fill), outline = cp_to_rgba(_outline);

        priv_vertex* vertices = aout_debug_draw_push_vertices(
                debug_draw,
                4,
                (priv_index[]) { 0, 1, 2, 0, 2, 3 },
                6
        );

        vertices[0] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, { -1, -1 }, r, fill, outline
        };
        vertices[1] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, { -1,  1 }, r, fill, outline
        };
        vertices[2] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, {  1,  1 }, r, fill, outline
        };
        vertices[3] = (priv_vertex) {
                { (float) pos.x, (float) pos.y }, {  1, -1 }, r, fill, outline
        };

        aout_debug_draw_segment(
                debug_draw,
                pos,
                cpvadd(pos, cpvmult(cpvforangle(angle), 0.75f * radius)),
                _outline
        );
}

void aout_debug_draw_segment(
                aout_debug_draw* debug_draw,
                cpVect a,
                cpVect b,
                cpSpaceDebugColor color) {
        aout_debug_draw_fat_segment(debug_draw, a, b, 0.0f, color, color);
}

void aout_debug_draw_fat_segment(
                aout_debug_draw* debug_draw,
                cpVect a,
                cpVect b,
                cpFloat radius,
                cpSpaceDebugColor _outline,
                cpSpaceDebugColor _fill) {
        static const priv_index indices[] = {
                0, 1, 2, 1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7
        };

        priv_vertex* vertices = aout_debug_draw_push_vertices(
                debug_draw, 8, indices, 18
        );

        cpVect t = cpvnormalize(cpvsub(b, a));

        float r = (float) radius + debug_draw->line_scale;
        priv_rgba8 fill = cp_to_rgba(_fill), outline = cp_to_rgba(_outline);

        vertices[0] = (priv_vertex) {
                { (float) a.x, (float) a.y },
                { (float) (-t.x + t.y), (float) (-t.x - t.y) },
                r, fill, outline
        };
        vertices[1] = (priv_vertex) {
                { (float) a.x, (float) a.y },
                { (float) (-t.x - t.y), (float) (+t.x - t.y) },
                r, fill, outline
        };
        vertices[2] = (priv_vertex) {
                { (float) a.x, (float) a.y },
                { (float) (-0.0 + t.y), (float) (-t.x + 0.0) },
                r, fill, outline
        };
        vertices[3] = (priv_vertex) {
                { (float) a.x, (float) a.y },
                { (float) (-0.0 - t.y), (float) (+t.x + 0.0) },
                r, fill, outline
        };
        vertices[4] = (priv_vertex) {
                { (float) b.x, (float) b.y },
                { (float) (+0.0 + t.y), (float) (-t.x - 0.0) },
                r, fill, outline
        };
        vertices[5] = (priv_vertex) {
                { (float) b.x, (float) b.y },
                { (float) (+0.0 - t.y), (float) (+t.x - 0.0) },
                r, fill, outline
        };
        vertices[6] = (priv_vertex) {
                { (float) b.x, (float) b.y },
                { (float) (+t.x + t.y), (float) (-t.x + t.y) },
                r, fill, outline
        };
        vertices[7] = (priv_vertex) {
                { (float) b.x, (float) b.y },
                { (float) (+t.x - t.y), (float) (+t.x + t.y) },
                r, fill, outline
        };
}

#define AOUT_MAX_POLY_VERTICES 64
// Fill needs (count - 2) triangles.
// Outline needs 4*count triangles.
#define AOUT_MAX_POLY_INDICES (3 * (5 * AOUT_MAX_POLY_VERTICES - 2))

void aout_debug_draw_polygon(
                aout_debug_draw* debug_draw,
                int count,
                const cpVect *verts,
                cpFloat radius,
                cpSpaceDebugColor _outline,
                cpSpaceDebugColor _fill) {
        priv_rgba8 fill = cp_to_rgba(_fill), outline = cp_to_rgba(_outline);
        outline = fill;
        outline.r *= 0.5f;
        outline.g *= 0.5f;
        outline.b *= 0.5f;

        priv_index indices[AOUT_MAX_POLY_INDICES];

        // Polygon fill triangles.
        for (int i = 0; i < count - 2; ++i) {
                indices[3 * i + 0] = 0;
                indices[3 * i + 1] = 4 * (i + 1);
                indices[3 * i + 2] = 4 * (i + 2);
        }

        // Polygon outline triangles.
        priv_index* cursor = indices + 3 * (count - 2);
        for (int i0 = 0; i0 < count; i0++) {
                int i1 = (i0 + 1) % count;
                cursor[12 * i0 +  0] = 4 * i0 + 0;
                cursor[12 * i0 +  1] = 4 * i0 + 1;
                cursor[12 * i0 +  2] = 4 * i0 + 2;
                cursor[12 * i0 +  3] = 4 * i0 + 0;
                cursor[12 * i0 +  4] = 4 * i0 + 2;
                cursor[12 * i0 +  5] = 4 * i0 + 3;
                cursor[12 * i0 +  6] = 4 * i0 + 0;
                cursor[12 * i0 +  7] = 4 * i0 + 3;
                cursor[12 * i0 +  8] = 4 * i1 + 0;
                cursor[12 * i0 +  9] = 4 * i0 + 3;
                cursor[12 * i0 + 10] = 4 * i1 + 0;
                cursor[12 * i0 + 11] = 4 * i1 + 1;
        }

        float inset = (float) -cpfmax(0, 2 * debug_draw->line_scale - radius);
        float outset = (float) radius + debug_draw->line_scale;
        float r = outset - inset;

        priv_vertex* vertices = aout_debug_draw_push_vertices(
                debug_draw,
                4 * count,
                indices,
                3 * (5 * count - 2)
        );

        for (int i = 0; i < count; i++) {
                cpVect v0 = verts[i];
                cpVect v_prev = verts[(i + (count - 1)) % count];
                cpVect v_next = verts[(i + (count + 1)) % count];

                cpVect n1 = cpvnormalize(cpvrperp(cpvsub(v0, v_prev)));
                cpVect n2 = cpvnormalize(cpvrperp(cpvsub(v_next, v0)));
                cpVect of = cpvmult(cpvadd(n1, n2), 1.0 / (cpvdot(n1, n2) + 1.0f));
                cpVect v = cpvadd(v0, cpvmult(of, inset));

                vertices[4 * i + 0] = (priv_vertex) {
                        { (float) v.x, (float) v.y },
                        { 0.0f, 0.0f },
                        0.0f, fill, outline
                };
                vertices[4 * i + 1] = (priv_vertex) {
                        { (float) v.x, (float) v.y },
                        { (float) n1.x, (float) n1.y },
                        r, fill, outline
                };
                vertices[4 * i + 2] = (priv_vertex) {
                        { (float) v.x, (float) v.y },
                        { (float) of.x, (float) of.y },
                        r, fill, outline
                };
                vertices[4 * i + 3] = (priv_vertex) {
                        { (float) v.x, (float) v.y },
                        { (float) n2.x, (float) n2.y },
                        r, fill, outline
                };
        }
}

void aout_debug_draw_bb(
                aout_debug_draw* debug_draw,
                cpBB bb,
                cpSpaceDebugColor color) {
        cpVect vertices[] = {
                cpv(bb.r, bb.b),
                cpv(bb.r, bb.t),
                cpv(bb.l, bb.t),
                cpv(bb.l, bb.b),
        };
        aout_debug_draw_polygon(
                debug_draw, 4, vertices, 0.0f, color, rgba_to_cp((priv_rgba8) {0})
        );
}

void aout_debug_draw_flush(
                aout_debug_draw* debug_draw) {
        cpTransform t = debug_draw->view_matrix;
        priv_view_matrix view_matrix = {
                .entries = {
                        (float) t.a , (float) t.b , 0.0f, 0.0f,
                        (float) t.c , (float) t.d , 0.0f, 0.0f,
                                0.0f,         0.0f, 1.0f, 0.0f,
                        (float) t.tx, (float) t.ty, 0.0f, 1.0f,
                },
        };

        sg_update_buffer(debug_draw->vertex_buffer, &(sg_range) {
                .ptr = debug_draw->vertices,
                .size = debug_draw->vertex_count * sizeof(priv_vertex),

        });
        sg_update_buffer(debug_draw->index_buffer, &(sg_range) {
                .ptr = debug_draw->indices,
                .size = debug_draw->index_count * sizeof(priv_index),
        });

        sg_apply_pipeline(debug_draw->pipeline);
        sg_apply_bindings(&debug_draw->bindings);
        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, &(sg_range) {
                .ptr = &view_matrix,
                .size = sizeof(priv_view_matrix),
        });
        sg_draw(0, debug_draw->index_count, 1);
}

void aout_debug_draw_clear(
                aout_debug_draw* debug_draw) {
        debug_draw->vertex_count = debug_draw->index_count = 0;
}
