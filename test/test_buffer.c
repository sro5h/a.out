#include <common/buffer.h>
#include <common/byte_order.h>
#include <greatest/greatest.h>

TEST buffer_write_u8(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        uint8_t u = 127;

        res = aout_buffer_write_u8(&buffer, u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], u);
        ASSERT_EQ(buffer.data[1], 0);
        ASSERT_EQ(buffer.data[2], 0);
        ASSERT_EQ(buffer.data[3], 0);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        res = aout_buffer_write_u8(&buffer, 1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], u);
        ASSERT_EQ(buffer.data[1], 1);
        ASSERT_EQ(buffer.data[2], 0);
        ASSERT_EQ(buffer.data[3], 0);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        res = aout_buffer_write_u8(&buffer, (uint8_t) -156);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], u);
        ASSERT_EQ(buffer.data[1], 1);
        ASSERT_EQ(buffer.data[2], (uint8_t) -156);
        ASSERT_EQ(buffer.data[3], 0);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 1);
        ASSERT(AOUT_IS_OK(res));

        ASSERT_EQ(buffer.data[0], u);
        ASSERT_EQ(buffer.data[1], 1);
        ASSERT_EQ(buffer.data[2], (uint8_t) -156);
        ASSERT_EQ(buffer.data[3], 1);
        ASSERT_EQ(buffer.data[4], 1);
        ASSERT_EQ(buffer.data[5], 1);
        ASSERT_EQ(buffer.data[6], 1);
        ASSERT_EQ(buffer.data[7], 1);
        ASSERT_EQ(buffer.data[8], 1);
        ASSERT_EQ(buffer.data[9], 1);

        res = aout_buffer_write_u8(&buffer, 1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);
        ASSERT_EQ(buffer.data[0], u);
        ASSERT_EQ(buffer.data[1], 1);
        ASSERT_EQ(buffer.data[2], (uint8_t) -156);
        ASSERT_EQ(buffer.data[3], 1);
        ASSERT_EQ(buffer.data[4], 1);
        ASSERT_EQ(buffer.data[5], 1);
        ASSERT_EQ(buffer.data[6], 1);
        ASSERT_EQ(buffer.data[7], 1);
        ASSERT_EQ(buffer.data[8], 1);
        ASSERT_EQ(buffer.data[9], 1);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_u16(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        uint16_t u0 = 16956;
        uint16_t h0 = aout_hton_u16(u0);

        res = aout_buffer_write_u16(&buffer, u0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], 0);
        ASSERT_EQ(buffer.data[3], 0);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        uint16_t u1 = -18329;
        uint16_t h1 = aout_hton_u16(u1);

        res = aout_buffer_write_u16(&buffer, u1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        uint16_t u2 = 1;
        uint16_t h2 = aout_hton_u16(u2);

        res = aout_buffer_write_u16(&buffer, u2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, u2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, u2);
        ASSERT(AOUT_IS_OK(res));

        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(buffer.data[8], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[9], ((uint8_t*) &h2)[1]);

        res = aout_buffer_write_u16(&buffer, u2);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(buffer.data[8], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[9], ((uint8_t*) &h2)[1]);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_u32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        uint32_t u0 = 169563290;
        uint32_t h0 = aout_hton_u32(u0);

        res = aout_buffer_write_u32(&buffer, u0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        uint32_t u1 = -169563290;
        uint32_t h1 = aout_hton_u32(u1);

        res = aout_buffer_write_u32(&buffer, u1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        res = aout_buffer_write_u32(&buffer, u1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_u64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };
        uint64_t u0 = 16956329032;
        uint64_t h0 = aout_hton_u64(u0);

        res = aout_buffer_write_u64(&buffer, u0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(buffer.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(buffer.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(buffer.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(buffer.data[ 8], 0);
        ASSERT_EQ(buffer.data[ 9], 0);
        ASSERT_EQ(buffer.data[10], 0);
        ASSERT_EQ(buffer.data[11], 0);
        ASSERT_EQ(buffer.data[12], 0);
        ASSERT_EQ(buffer.data[13], 0);
        ASSERT_EQ(buffer.data[14], 0);
        ASSERT_EQ(buffer.data[15], 0);
        ASSERT_EQ(buffer.data[16], 0);
        ASSERT_EQ(buffer.data[17], 0);
        ASSERT_EQ(buffer.data[18], 0);

        uint64_t u1 = -16956329032;
        uint64_t h1 = aout_hton_u64(u1);

        res = aout_buffer_write_u64(&buffer, u1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(buffer.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(buffer.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(buffer.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(buffer.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(buffer.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(buffer.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(buffer.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(buffer.data[16], 0);
        ASSERT_EQ(buffer.data[17], 0);
        ASSERT_EQ(buffer.data[18], 0);

        res = aout_buffer_write_u64(&buffer, u1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);
        ASSERT_EQ(buffer.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(buffer.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(buffer.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(buffer.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(buffer.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(buffer.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(buffer.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(buffer.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(buffer.data[16], 0);
        ASSERT_EQ(buffer.data[17], 0);
        ASSERT_EQ(buffer.data[18], 0);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_f32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        float32_t f0 = 3.14159f;
        uint32_t  h0 = aout_hton_f32(f0);

        res = aout_buffer_write_f32(&buffer, f0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[4], 0);
        ASSERT_EQ(buffer.data[5], 0);
        ASSERT_EQ(buffer.data[6], 0);
        ASSERT_EQ(buffer.data[7], 0);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        float32_t f1 = -3.14159f;
        uint32_t  h1 = aout_hton_f32(f1);

        res = aout_buffer_write_f32(&buffer, f1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        res = aout_buffer_write_f32(&buffer, f1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);
        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[8], 0);
        ASSERT_EQ(buffer.data[9], 0);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_f64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };
        float64_t f0 = 3.1415926535;
        uint64_t  h0 = aout_hton_f64(f0);

        res = aout_buffer_write_f64(&buffer, f0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(buffer.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(buffer.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(buffer.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(buffer.data[ 8], 0);
        ASSERT_EQ(buffer.data[ 9], 0);
        ASSERT_EQ(buffer.data[10], 0);
        ASSERT_EQ(buffer.data[11], 0);
        ASSERT_EQ(buffer.data[12], 0);
        ASSERT_EQ(buffer.data[13], 0);
        ASSERT_EQ(buffer.data[14], 0);
        ASSERT_EQ(buffer.data[15], 0);
        ASSERT_EQ(buffer.data[16], 0);
        ASSERT_EQ(buffer.data[17], 0);
        ASSERT_EQ(buffer.data[18], 0);

        float64_t f1 = -3.1415926535;
        uint64_t  h1 = aout_hton_f64(f1);

        res = aout_buffer_write_f64(&buffer, f1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(buffer.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(buffer.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(buffer.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(buffer.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(buffer.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(buffer.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(buffer.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(buffer.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(buffer.data[16], 0);
        ASSERT_EQ(buffer.data[17], 0);
        ASSERT_EQ(buffer.data[18], 0);

        res = aout_buffer_write_f64(&buffer, f1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);
        ASSERT_EQ(buffer.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(buffer.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(buffer.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(buffer.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(buffer.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(buffer.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(buffer.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(buffer.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(buffer.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(buffer.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(buffer.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(buffer.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(buffer.data[16], 0);
        ASSERT_EQ(buffer.data[17], 0);
        ASSERT_EQ(buffer.data[18], 0);

        free(data);
        data = NULL;
        PASS();
}

SUITE(test_buffer) {
        RUN_TEST(buffer_write_u8);
        RUN_TEST(buffer_write_u16);
        RUN_TEST(buffer_write_u32);
        RUN_TEST(buffer_write_u64);
        RUN_TEST(buffer_write_f32);
        RUN_TEST(buffer_write_f64);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_buffer);

        GREATEST_MAIN_END();
}
