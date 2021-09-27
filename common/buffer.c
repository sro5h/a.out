#include "buffer.h"

#include "byte_order.h"
#include <string.h>

aout_res aout_buffer_write_u8(
                aout_buffer* buffer,
                uint8_t value) {
        return aout_buffer_write_bytes(buffer, &value, 1);
}

aout_res aout_buffer_write_u16(
                aout_buffer* buffer,
                uint16_t value) {
        value = aout_hton_u16(value);
        // Casting to uint8_t* is allowed
        return aout_buffer_write_bytes(buffer, (uint8_t*) &value, sizeof(value));
}

aout_res aout_buffer_write_u32(
                aout_buffer* buffer,
                uint32_t value) {
        value = aout_hton_u32(value);
        return aout_buffer_write_bytes(buffer, (uint8_t*) &value, sizeof(value));
}

aout_res aout_buffer_write_u64(
                aout_buffer* buffer,
                uint64_t value) {
        value = aout_hton_u64(value);
        return aout_buffer_write_bytes(buffer, (uint8_t*) &value, sizeof(value));
}

aout_res aout_buffer_write_f32(
                aout_buffer* buffer,
                float32_t value) {
        uint32_t tmp = aout_hton_f32(value);
        return aout_buffer_write_bytes(buffer, (uint8_t*) &tmp, sizeof(tmp));
}

aout_res aout_buffer_write_f64(
                aout_buffer* buffer,
                float64_t value) {
        uint64_t tmp = aout_hton_f64(value);
        return aout_buffer_write_bytes(buffer, (uint8_t*) &tmp, sizeof(tmp));
}

aout_res aout_buffer_write_bytes(
                aout_buffer* buffer,
                uint8_t* values,
                size_t size) {
        assert(buffer); assert(values);
        assert(buffer->index <= buffer->data_size); // TODO: Treat as error?

        if (buffer->index + size > buffer->data_size) {
                return AOUT_ERR(AOUT_BUFFER_ERR_FULL);
        }

        memcpy(&buffer->data[buffer->index], values, size);
        buffer->index += size;

        return AOUT_OK;
}

aout_res aout_buffer_read_u8(
                aout_buffer* buffer,
                uint8_t* value) {
        return aout_buffer_read_bytes(buffer, value, sizeof(*value));
}

aout_res aout_buffer_read_u16(
                aout_buffer* buffer,
                uint16_t* value) {
        aout_res res = aout_buffer_read_bytes(buffer, (uint8_t*) value,
                        sizeof(*value));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_u16(*value);
        }

        return res;
}

aout_res aout_buffer_read_u32(
                aout_buffer* buffer,
                uint32_t* value) {
        aout_res res = aout_buffer_read_bytes(buffer, (uint8_t*) value,
                        sizeof(*value));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_u32(*value);
        }

        return res;
}

aout_res aout_buffer_read_u64(
                aout_buffer* buffer,
                uint64_t* value) {
        aout_res res = aout_buffer_read_bytes(buffer, (uint8_t*) value,
                        sizeof(*value));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_u64(*value);
        }

        return res;
}

aout_res aout_buffer_read_f32(
                aout_buffer* buffer,
                float32_t* value) {
        assert(value);

        uint32_t tmp;
        aout_res res = aout_buffer_read_bytes(buffer, (uint8_t*) &tmp,
                        sizeof(tmp));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_f32(tmp);
        }

        return res;
}

aout_res aout_buffer_read_f64(
                aout_buffer* buffer,
                float64_t* value) {
        assert(value);

        uint64_t tmp;
        aout_res res = aout_buffer_read_bytes(buffer, (uint8_t*) &tmp,
                        sizeof(tmp));

        if (AOUT_IS_OK(res)) {
                *value = aout_ntoh_f64(tmp);
        }

        return res;
}

aout_res aout_buffer_read_bytes(
                aout_buffer* buffer,
                uint8_t* values,
                size_t size) {
        assert(buffer); assert(values);
        assert(buffer->index <= buffer->data_size);

        if (buffer->index + size > buffer->data_size) {
                // TODO: Add new error code
                return AOUT_ERR(AOUT_BUFFER_ERR_FULL);
        }

        memcpy(values, &buffer->data[buffer->index], size);
        buffer->index += size;

        return AOUT_OK;
}
