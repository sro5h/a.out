#ifndef COMMON_STREAM_H
#define COMMON_STREAM_H

#include "result.h"
#include "types.h"

typedef struct aout_stream {
        uint8_t* data;
        size_t data_size;
        size_t index;
} aout_stream;

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
                uint8_t* values,
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

COMMON_API size_t aout_stream_get_count(
                aout_stream* self);

COMMON_API void aout_stream_reset(
                aout_stream* self);

#endif
