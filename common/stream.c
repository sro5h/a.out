#include "stream.h"

#include "byte_order.h"
#include <string.h>

aout_res aout_stream_write_u8(
                aout_stream* stream,
                uint8_t value) {
        return aout_stream_write_bytes(stream, &value, 1);
}

aout_res aout_stream_write_u16(
                aout_stream* stream,
                uint16_t value) {
        value = aout_hton_u16(value);
        // Casting to uint8_t* is allowed
        return aout_stream_write_bytes(stream, (uint8_t*) &value, sizeof(value));
}

aout_res aout_stream_write_u32(
                aout_stream* stream,
                uint32_t value) {
        value = aout_hton_u32(value);
        return aout_stream_write_bytes(stream, (uint8_t*) &value, sizeof(value));
}

aout_res aout_stream_write_u64(
                aout_stream* stream,
                uint64_t value) {
        value = aout_hton_u64(value);
        return aout_stream_write_bytes(stream, (uint8_t*) &value, sizeof(value));
}

aout_res aout_stream_write_f32(
                aout_stream* stream,
                float32_t value) {
        uint32_t tmp = aout_hton_f32(value);
        return aout_stream_write_bytes(stream, (uint8_t*) &tmp, sizeof(tmp));
}

aout_res aout_stream_write_f64(
                aout_stream* stream,
                float64_t value) {
        uint64_t tmp = aout_hton_f64(value);
        return aout_stream_write_bytes(stream, (uint8_t*) &tmp, sizeof(tmp));
}

aout_res aout_stream_write_bytes(
                aout_stream* stream,
                uint8_t* values,
                size_t size) {
        assert(stream); assert(values);

        if (!aout_stream_has_capacity(stream, size)) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        memcpy(&stream->data[stream->index], values, size);
        stream->index += size;

        return AOUT_OK;
}

aout_res aout_stream_read_u8(
                aout_stream* stream,
                uint8_t* value) {
        return aout_stream_read_bytes(stream, value, sizeof(*value));
}

aout_res aout_stream_read_u16(
                aout_stream* stream,
                uint16_t* value) {
        aout_res res = aout_stream_read_bytes(stream, (uint8_t*) value,
                        sizeof(*value));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_u16(*value);
        }

        return res;
}

aout_res aout_stream_read_u32(
                aout_stream* stream,
                uint32_t* value) {
        aout_res res = aout_stream_read_bytes(stream, (uint8_t*) value,
                        sizeof(*value));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_u32(*value);
        }

        return res;
}

aout_res aout_stream_read_u64(
                aout_stream* stream,
                uint64_t* value) {
        aout_res res = aout_stream_read_bytes(stream, (uint8_t*) value,
                        sizeof(*value));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_u64(*value);
        }

        return res;
}

aout_res aout_stream_read_f32(
                aout_stream* stream,
                float32_t* value) {
        assert(value);

        uint32_t tmp;
        aout_res res = aout_stream_read_bytes(stream, (uint8_t*) &tmp,
                        sizeof(tmp));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_f32(tmp);
        }

        return res;
}

aout_res aout_stream_read_f64(
                aout_stream* stream,
                float64_t* value) {
        assert(value);

        uint64_t tmp;
        aout_res res = aout_stream_read_bytes(stream, (uint8_t*) &tmp,
                        sizeof(tmp));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_f64(tmp);
        }

        return res;
}

aout_res aout_stream_read_bytes(
                aout_stream* stream,
                uint8_t* values,
                size_t size) {
        assert(stream); assert(values);

        if (!aout_stream_has_capacity(stream, size)) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        memcpy(values, &stream->data[stream->index], size);
        stream->index += size;

        return AOUT_OK;
}

// If aout_stream_has_capacity(stream, size) returns true a call to
// aout_stream_write_bytes(stream, data, size) will never fail. Thus a call to
// aout_stream_write_*(stream, value) will never fail if sizeof(value) is less
// than or equal to size.
// The same is true for the read functions.
bool aout_stream_has_capacity(
                aout_stream* stream,
                size_t size) {
        assert(stream);
        assert(stream->index <= stream->data_size); // TODO: Treat as error?
        // No don't treat that as an error. The index should only be modified
        // by aout_stream_* functions and thus never be invalid.

        return (stream->index + size) <= stream->data_size;
}

size_t aout_stream_get_count(
                aout_stream* stream) {
        assert(stream);
        return stream->index <= stream->data_size ?
                        stream->index : stream->data_size;
}

void aout_stream_reset(
                aout_stream* stream) {
        assert(stream);
        stream->index = 0;
}
