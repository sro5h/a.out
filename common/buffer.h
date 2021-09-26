#ifndef COMMON_BUFFER_H
#define COMMON_BUFFER_H

#include "result.h"
#include "types.h"

typedef struct aout_buffer {
        uint8_t* data; // TODO: Add data type ubyte_t?
        size_t data_size;
        size_t index;
} aout_buffer;

typedef enum aout_buffer_res {
        AOUT_BUFFER_OK,
        AOUT_BUFFER_ERR_FULL,
} aout_buffer_res;

COMMON_API aout_res aout_buffer_write_u8(
                aout_buffer* buffer,
                uint8_t value);

COMMON_API aout_res aout_buffer_write_u16(
                aout_buffer* buffer,
                uint16_t value);

COMMON_API aout_res aout_buffer_write_u32(
                aout_buffer* buffer,
                uint32_t value);

COMMON_API aout_res aout_buffer_write_u64(
                aout_buffer* buffer,
                uint64_t value);

COMMON_API aout_res aout_buffer_write_f32(
                aout_buffer* buffer,
                float32_t value);

COMMON_API aout_res aout_buffer_write_f64(
                aout_buffer* buffer,
                float64_t value);

COMMON_API aout_res aout_buffer_write_bytes(
                aout_buffer* buffer,
                uint8_t* values, // TODO: Add data type ubyte_t?
                size_t size);

// TODO: Add read function variants

#endif
