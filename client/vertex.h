#ifndef CLIENT_VERTEX_H
#define CLIENT_VERTEX_H

#include <common/types.h>

typedef struct aout_float2 {
        float32_t x, y;
} aout_float2;

typedef struct aout_rgba8 {
        uint8_t r, g, b, a;
} aout_rgba8;

// Geometry vertex
typedef struct aout_vertex {
        aout_float2 position;
        aout_float2 uv;
        float32_t radius;
        aout_rgba8 color_fill;
        aout_rgba8 color_outline;
} aout_vertex;

typedef uint16_t aout_index;

#endif
