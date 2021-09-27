#ifndef COMMON_BUFFER_H
#define COMMON_BUFFER_H

#include "result.h"
#include "types.h"

// TODO: Add writer and reader structs that encapsulate a buffer and its
// current read or write index.
typedef struct aout_buffer {
        uint8_t* data; // TODO: Add data type ubyte_t?
        size_t data_size;
        size_t index;
} aout_buffer;

#if 0
typedef struct aout_buffer_writer {
        aout_buffer buffer; // struct { uint8_t* data; size_t data_size; };
        size_t index;
} aout_buffer_writer;

aout_buffer_writer writer = {
        .buffer.data = data,
        .buffer.data_size = size
};

aout_buffer_writer aout_buffer_writer_make(
                aout_buffer buffer);

void aout_buffer_writer_reset(
                aout_buffer_writer* writer) {
        writer->index = 0;
}
#endif

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

COMMON_API aout_res aout_buffer_read_u8(
                aout_buffer* buffer,
                uint8_t* value);

COMMON_API aout_res aout_buffer_read_u16(
                aout_buffer* buffer,
                uint16_t* value);

COMMON_API aout_res aout_buffer_read_u32(
                aout_buffer* buffer,
                uint32_t* value);

COMMON_API aout_res aout_buffer_read_u64(
                aout_buffer* buffer,
                uint64_t* value);

COMMON_API aout_res aout_buffer_read_f32(
                aout_buffer* buffer,
                float32_t* value);

COMMON_API aout_res aout_buffer_read_f64(
                aout_buffer* buffer,
                float64_t* value);

COMMON_API aout_res aout_buffer_read_bytes(
                aout_buffer* buffer,
                uint8_t* values,
                size_t size);

#endif
