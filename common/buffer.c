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
        assert(buffer);
        assert(buffer->index <= buffer->data_size); // TODO: Treat as error?

        if (buffer->index + size > buffer->data_size) {
                return AOUT_ERR(AOUT_BUFFER_ERR_FULL);
        }

        memcpy(&buffer->data[buffer->index], values, size);
        buffer->index += size;

        return AOUT_OK;
}
