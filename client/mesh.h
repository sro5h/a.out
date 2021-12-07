#ifndef CLIENT_MESH_H
#define CLIENT_MESH_H

#include <common/types.h>

#include <sokol/sokol_gfx.h>

typedef struct aout_mesh {
        sg_buffer vertex_buffer, index_buffer;
        size_t index_count;
        // TODO: Don't create per mesh
        sg_pipeline pipeline;
} aout_mesh;

#endif
