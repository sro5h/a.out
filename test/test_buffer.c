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

TEST buffer_read_u8(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_buffer buffer = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint8_t u = 0;

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[0]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[1]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[2]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[3]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[4]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[5]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[6]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[7]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[8]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[9]);

        res = aout_buffer_read_u8(&buffer, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        PASS();
}

TEST buffer_read_u16(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_buffer buffer = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint16_t u = 0;
        uint16_t h = aout_ntoh_u16(*((uint16_t*) &data[0]));

        res = aout_buffer_read_u16(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_u16(*((uint16_t*) &data[2]));

        res = aout_buffer_read_u16(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_u16(*((uint16_t*) &data[4]));

        res = aout_buffer_read_u16(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_u16(*((uint16_t*) &data[6]));

        res = aout_buffer_read_u16(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_u16(*((uint16_t*) &data[8]));

        res = aout_buffer_read_u16(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        res = aout_buffer_read_u16(&buffer, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        PASS();
}

TEST buffer_read_u32(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_buffer buffer = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint32_t u = 0;
        uint32_t h = aout_ntoh_u32(*((uint32_t*) &data[0]));

        res = aout_buffer_read_u32(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_u32(*((uint32_t*) &data[4]));

        res = aout_buffer_read_u32(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        res = aout_buffer_read_u32(&buffer, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        PASS();
}

TEST buffer_read_u64(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3, 31, 221, 169, 64, 98, 132, 21, 17, 83 };
        aout_buffer buffer = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint64_t u = 0;
        uint64_t h = aout_ntoh_u64(*((uint64_t*) &data[0]));

        res = aout_buffer_read_u64(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_u64(*((uint64_t*) &data[8]));

        res = aout_buffer_read_u64(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        res = aout_buffer_read_u64(&buffer, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        PASS();
}

TEST buffer_read_f32(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_buffer buffer = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        float32_t u = 0;
        float32_t h = aout_ntoh_f32(*((uint32_t*) &data[0]));

        res = aout_buffer_read_f32(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_f32(*((uint32_t*) &data[4]));

        res = aout_buffer_read_f32(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        res = aout_buffer_read_f32(&buffer, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        PASS();
}

TEST buffer_read_f64(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3, 31, 221, 169, 64, 98, 132, 21, 17, 83 };
        aout_buffer buffer = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        float64_t u = 0;
        float64_t h = aout_ntoh_f64(*((uint64_t*) &data[0]));

        res = aout_buffer_read_f64(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        h = aout_ntoh_f64(*((uint64_t*) &data[8]));

        res = aout_buffer_read_f64(&buffer, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);

        res = aout_buffer_read_f64(&buffer, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        PASS();
}

TEST buffer_write_u8_then_read_u8(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_buffer_write_u8(&buffer, 21);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 139);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, -91);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 47);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 245);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 101);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 16);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 78);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 20);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u8(&buffer, 23);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(buffer.data[0], 21);
        ASSERT_EQ(buffer.data[1], 139);
        ASSERT_EQ(buffer.data[2], 2);
        ASSERT_EQ(buffer.data[3], (uint8_t) -91);
        ASSERT_EQ(buffer.data[4], 47);
        ASSERT_EQ(buffer.data[5], 245);
        ASSERT_EQ(buffer.data[6], 101);
        ASSERT_EQ(buffer.data[7], 16);
        ASSERT_EQ(buffer.data[8], 78);
        ASSERT_EQ(buffer.data[9], 20);

        uint8_t results[10] = { 0 };
        aout_buffer_reset(&buffer);

        res = aout_buffer_read_u8(&buffer, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[2]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[3]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[4]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[5]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[6]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[7]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[8]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[9]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u8(&buffer, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(results[0], 21);
        ASSERT_EQ(results[1], 139);
        ASSERT_EQ(results[2], 2);
        ASSERT_EQ(results[3], (uint8_t) -91);
        ASSERT_EQ(results[4], 47);
        ASSERT_EQ(results[5], 245);
        ASSERT_EQ(results[6], 101);
        ASSERT_EQ(results[7], 16);
        ASSERT_EQ(results[8], 78);
        ASSERT_EQ(results[9], 20);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_u16_then_read_u16(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_buffer_write_u16(&buffer, 21000);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, 1399);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, 2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, -915);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, 47);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u16(&buffer, 2339);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        uint16_t h0 = aout_hton_u16(21000);
        uint16_t h1 = aout_hton_u16(1399);
        uint16_t h2 = aout_hton_u16(2);
        uint16_t h3 = aout_hton_u16(-915);
        uint16_t h4 = aout_hton_u16(47);

        ASSERT_EQ(buffer.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(buffer.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(buffer.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(buffer.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(buffer.data[4], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(buffer.data[5], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(buffer.data[6], ((uint8_t*) &h3)[0]);
        ASSERT_EQ(buffer.data[7], ((uint8_t*) &h3)[1]);
        ASSERT_EQ(buffer.data[8], ((uint8_t*) &h4)[0]);
        ASSERT_EQ(buffer.data[9], ((uint8_t*) &h4)[1]);

        uint16_t results[5] = { 0 };
        aout_buffer_reset(&buffer);

        res = aout_buffer_read_u16(&buffer, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u16(&buffer, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u16(&buffer, &results[2]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u16(&buffer, &results[3]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u16(&buffer, &results[4]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u16(&buffer, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(results[0], 21000);
        ASSERT_EQ(results[1], 1399);
        ASSERT_EQ(results[2], 2);
        ASSERT_EQ(results[3], (uint16_t) -915);
        ASSERT_EQ(results[4], 47);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_u32_then_read_u32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_buffer_write_u32(&buffer, 210000);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u32(&buffer, -915);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u32(&buffer, 2339);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        uint32_t h0 = aout_hton_u32(210000);
        uint32_t h1 = aout_hton_u32(-915);

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

        uint32_t results[2] = { 0 };
        aout_buffer_reset(&buffer);

        res = aout_buffer_read_u32(&buffer, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u32(&buffer, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u32(&buffer, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(results[0], 210000);
        ASSERT_EQ(results[1], (uint32_t) -915);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_u64_then_read_u64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };

        res = aout_buffer_write_u64(&buffer, 21000000);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u64(&buffer, -915);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_u64(&buffer, 2339);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        uint64_t h0 = aout_hton_u64(21000000);
        uint64_t h1 = aout_hton_u64(-915);

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

        uint64_t results[2] = { 0 };
        aout_buffer_reset(&buffer);

        res = aout_buffer_read_u64(&buffer, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u64(&buffer, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_u64(&buffer, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(results[0], 21000000);
        ASSERT_EQ(results[1], (uint64_t) -915);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_f32_then_read_f32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_buffer_write_f32(&buffer, 3.14159f);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_f32(&buffer, -3.14159f);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_f32(&buffer, 0.001f);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        uint32_t h0 = aout_hton_f32(3.14159f);
        uint32_t h1 = aout_hton_f32(-3.14159f);

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

        float32_t results[2] = { 0 };
        aout_buffer_reset(&buffer);

        res = aout_buffer_read_f32(&buffer, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_f32(&buffer, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_f32(&buffer, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(results[0], 3.14159f);
        ASSERT_EQ(results[1], -3.14159f);

        free(data);
        data = NULL;
        PASS();
}

TEST buffer_write_f64_then_read_f64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_buffer buffer = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };

        res = aout_buffer_write_f64(&buffer, 3.1415926535);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_f64(&buffer, -3.1415926535);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_write_f64(&buffer, 2339.0001);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        uint64_t h0 = aout_hton_f64(3.1415926535);
        uint64_t h1 = aout_hton_f64(-3.1415926535);

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

        float64_t results[2] = { 0 };
        aout_buffer_reset(&buffer);

        res = aout_buffer_read_f64(&buffer, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_f64(&buffer, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_buffer_read_f64(&buffer, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_BUFFER_ERR_FULL);

        ASSERT_EQ(results[0], 3.1415926535);
        ASSERT_EQ(results[1], -3.1415926535);

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

        RUN_TEST(buffer_read_u8);
        RUN_TEST(buffer_read_u16);
        RUN_TEST(buffer_read_u32);
        RUN_TEST(buffer_read_u64);
        RUN_TEST(buffer_read_f32);
        RUN_TEST(buffer_read_f64);

        RUN_TEST(buffer_write_u8_then_read_u8);
        RUN_TEST(buffer_write_u16_then_read_u16);
        RUN_TEST(buffer_write_u32_then_read_u32);
        RUN_TEST(buffer_write_u64_then_read_u64);
        RUN_TEST(buffer_write_f32_then_read_f32);
        RUN_TEST(buffer_write_f64_then_read_f64);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_buffer);

        GREATEST_MAIN_END();
}
