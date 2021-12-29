#ifndef COMMON_STREAM_H
#define COMMON_STREAM_H

#include "result.h"
#include "types.h"

// TODO: Add writer and reader structs that encapsulate a buffer and its
// current read or write index.
typedef struct aout_stream {
        uint8_t* data; // TODO: Add data type ubyte_t?
        size_t data_size;
        size_t index;
} aout_stream;

typedef enum aout_stream_res {
        AOUT_STREAM_OK,
        AOUT_STREAM_ERR_END_REACHED,
} aout_stream_res;

COMMON_API aout_res aout_stream_write_u8(
                aout_stream* self,
                uint8_t value);

COMMON_API aout_res aout_stream_write_u16(
                aout_stream* self,
                uint16_t value);

COMMON_API aout_res aout_stream_write_u32(
                aout_stream* self,
                uint32_t value);

COMMON_API aout_res aout_stream_write_u64(
                aout_stream* self,
                uint64_t value);

COMMON_API aout_res aout_stream_write_f32(
                aout_stream* self,
                float32_t value);

COMMON_API aout_res aout_stream_write_f64(
                aout_stream* self,
                float64_t value);

COMMON_API aout_res aout_stream_write_bytes(
                aout_stream* self,
                uint8_t* values, // TODO: Add data type ubyte_t?
                size_t size);

COMMON_API aout_res aout_stream_read_u8(
                aout_stream* self,
                uint8_t* value);

COMMON_API aout_res aout_stream_read_u16(
                aout_stream* self,
                uint16_t* value);

COMMON_API aout_res aout_stream_read_u32(
                aout_stream* self,
                uint32_t* value);

COMMON_API aout_res aout_stream_read_u64(
                aout_stream* self,
                uint64_t* value);

COMMON_API aout_res aout_stream_read_f32(
                aout_stream* self,
                float32_t* value);

COMMON_API aout_res aout_stream_read_f64(
                aout_stream* self,
                float64_t* value);

COMMON_API aout_res aout_stream_read_bytes(
                aout_stream* self,
                uint8_t* values,
                size_t size);

COMMON_API bool aout_stream_has_capacity(
                aout_stream* self,
                size_t size);
// TODO: Maybe rename
// bool aout_stream_query_capacity
// bool aout_stream_query_overflow

COMMON_API size_t aout_stream_get_count(
                aout_stream* self);

COMMON_API void aout_stream_reset( // TODO: Find better name
                aout_stream* self);

#endif
