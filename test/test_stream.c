#include <common/byte_order.h>
#include <common/stream.h>
#include <greatest/greatest.h>

TEST stream_write_u8(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        uint8_t u = 127;

        res = aout_stream_write_u8(&stream, u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], u);
        ASSERT_EQ(stream.data[1], 0);
        ASSERT_EQ(stream.data[2], 0);
        ASSERT_EQ(stream.data[3], 0);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 1);

        res = aout_stream_write_u8(&stream, 1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], u);
        ASSERT_EQ(stream.data[1], 1);
        ASSERT_EQ(stream.data[2], 0);
        ASSERT_EQ(stream.data[3], 0);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 2);

        res = aout_stream_write_u8(&stream, (uint8_t) -156);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], u);
        ASSERT_EQ(stream.data[1], 1);
        ASSERT_EQ(stream.data[2], (uint8_t) -156);
        ASSERT_EQ(stream.data[3], 0);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 3);

        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 1);
        ASSERT(AOUT_IS_OK(res));

        ASSERT_EQ(stream.data[0], u);
        ASSERT_EQ(stream.data[1], 1);
        ASSERT_EQ(stream.data[2], (uint8_t) -156);
        ASSERT_EQ(stream.data[3], 1);
        ASSERT_EQ(stream.data[4], 1);
        ASSERT_EQ(stream.data[5], 1);
        ASSERT_EQ(stream.data[6], 1);
        ASSERT_EQ(stream.data[7], 1);
        ASSERT_EQ(stream.data[8], 1);
        ASSERT_EQ(stream.data[9], 1);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        res = aout_stream_write_u8(&stream, 1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], u);
        ASSERT_EQ(stream.data[1], 1);
        ASSERT_EQ(stream.data[2], (uint8_t) -156);
        ASSERT_EQ(stream.data[3], 1);
        ASSERT_EQ(stream.data[4], 1);
        ASSERT_EQ(stream.data[5], 1);
        ASSERT_EQ(stream.data[6], 1);
        ASSERT_EQ(stream.data[7], 1);
        ASSERT_EQ(stream.data[8], 1);
        ASSERT_EQ(stream.data[9], 1);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_u16(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        uint16_t u0 = 16956;
        uint16_t h0 = aout_hton_u16(u0);

        res = aout_stream_write_u16(&stream, u0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], 0);
        ASSERT_EQ(stream.data[3], 0);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 2);

        uint16_t u1 = -18329;
        uint16_t h1 = aout_hton_u16(u1);

        res = aout_stream_write_u16(&stream, u1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        uint16_t u2 = 1;
        uint16_t h2 = aout_hton_u16(u2);

        res = aout_stream_write_u16(&stream, u2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, u2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, u2);
        ASSERT(AOUT_IS_OK(res));

        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(stream.data[8], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[9], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        res = aout_stream_write_u16(&stream, u2);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(stream.data[8], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[9], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_u32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        uint32_t u0 = 169563290;
        uint32_t h0 = aout_hton_u32(u0);

        res = aout_stream_write_u32(&stream, u0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        uint32_t u1 = -169563290;
        uint32_t h1 = aout_hton_u32(u1);

        res = aout_stream_write_u32(&stream, u1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        res = aout_stream_write_u32(&stream, u1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_u64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };
        uint64_t u0 = 16956329032;
        uint64_t h0 = aout_hton_u64(u0);

        res = aout_stream_write_u64(&stream, u0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], 0);
        ASSERT_EQ(stream.data[ 9], 0);
        ASSERT_EQ(stream.data[10], 0);
        ASSERT_EQ(stream.data[11], 0);
        ASSERT_EQ(stream.data[12], 0);
        ASSERT_EQ(stream.data[13], 0);
        ASSERT_EQ(stream.data[14], 0);
        ASSERT_EQ(stream.data[15], 0);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        uint64_t u1 = -16956329032;
        uint64_t h1 = aout_hton_u64(u1);

        res = aout_stream_write_u64(&stream, u1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(stream.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(stream.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(stream.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        res = aout_stream_write_u64(&stream, u1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(stream.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(stream.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(stream.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_f32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        float32_t f0 = 3.14159f;
        uint32_t  h0 = aout_hton_f32(f0);

        res = aout_stream_write_f32(&stream, f0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        float32_t f1 = -3.14159f;
        uint32_t  h1 = aout_hton_f32(f1);

        res = aout_stream_write_f32(&stream, f1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        res = aout_stream_write_f32(&stream, f1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_f64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };
        float64_t f0 = 3.1415926535;
        uint64_t  h0 = aout_hton_f64(f0);

        res = aout_stream_write_f64(&stream, f0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], 0);
        ASSERT_EQ(stream.data[ 9], 0);
        ASSERT_EQ(stream.data[10], 0);
        ASSERT_EQ(stream.data[11], 0);
        ASSERT_EQ(stream.data[12], 0);
        ASSERT_EQ(stream.data[13], 0);
        ASSERT_EQ(stream.data[14], 0);
        ASSERT_EQ(stream.data[15], 0);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        float64_t f1 = -3.1415926535;
        uint64_t  h1 = aout_hton_f64(f1);

        res = aout_stream_write_f64(&stream, f1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(stream.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(stream.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(stream.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        res = aout_stream_write_f64(&stream, f1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(stream.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(stream.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(stream.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_read_u8(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint8_t u = 0;

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[0]);
        ASSERT_EQ(aout_stream_get_count(&stream), 1);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[1]);
        ASSERT_EQ(aout_stream_get_count(&stream), 2);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[2]);
        ASSERT_EQ(aout_stream_get_count(&stream), 3);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[3]);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[4]);
        ASSERT_EQ(aout_stream_get_count(&stream), 5);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[5]);
        ASSERT_EQ(aout_stream_get_count(&stream), 6);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[6]);
        ASSERT_EQ(aout_stream_get_count(&stream), 7);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[7]);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[8]);
        ASSERT_EQ(aout_stream_get_count(&stream), 9);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, data[9]);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        res = aout_stream_read_u8(&stream, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        PASS();
}

TEST stream_read_u16(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint16_t u = 0;
        uint16_t h = aout_ntoh_u16(*((uint16_t*) &data[0]));

        res = aout_stream_read_u16(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 2);

        h = aout_ntoh_u16(*((uint16_t*) &data[2]));

        res = aout_stream_read_u16(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        h = aout_ntoh_u16(*((uint16_t*) &data[4]));

        res = aout_stream_read_u16(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 6);

        h = aout_ntoh_u16(*((uint16_t*) &data[6]));

        res = aout_stream_read_u16(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        h = aout_ntoh_u16(*((uint16_t*) &data[8]));

        res = aout_stream_read_u16(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        res = aout_stream_read_u16(&stream, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        PASS();
}

TEST stream_read_u32(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint32_t u = 0;
        uint32_t h = aout_ntoh_u32(*((uint32_t*) &data[0]));

        res = aout_stream_read_u32(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        h = aout_ntoh_u32(*((uint32_t*) &data[4]));

        res = aout_stream_read_u32(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        res = aout_stream_read_u32(&stream, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        PASS();
}

TEST stream_read_u64(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3, 31, 221, 169, 64, 98, 132, 21, 17, 83 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        uint64_t u = 0;
        uint64_t h = aout_ntoh_u64(*((uint64_t*) &data[0]));

        res = aout_stream_read_u64(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        h = aout_ntoh_u64(*((uint64_t*) &data[8]));

        res = aout_stream_read_u64(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        res = aout_stream_read_u64(&stream, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        PASS();
}

TEST stream_read_f32(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        float32_t u = 0;
        float32_t h = aout_ntoh_f32(*((uint32_t*) &data[0]));

        res = aout_stream_read_f32(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 4);

        h = aout_ntoh_f32(*((uint32_t*) &data[4]));

        res = aout_stream_read_f32(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        res = aout_stream_read_f32(&stream, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        PASS();
}

TEST stream_read_f64(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3, 31, 221, 169, 64, 98, 132, 21, 17, 83 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        float64_t u = 0;
        float64_t h = aout_ntoh_f64(*((uint64_t*) &data[0]));

        res = aout_stream_read_f64(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        h = aout_ntoh_f64(*((uint64_t*) &data[8]));

        res = aout_stream_read_f64(&stream, &u);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(u, h);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        res = aout_stream_read_f64(&stream, &u);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        PASS();
}

TEST stream_write_u8_then_read_u8(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_stream_write_u8(&stream, 21);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 139);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, -91);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 47);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 245);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 101);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 16);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 78);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 20);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u8(&stream, 23);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        ASSERT_EQ(stream.data[0], 21);
        ASSERT_EQ(stream.data[1], 139);
        ASSERT_EQ(stream.data[2], 2);
        ASSERT_EQ(stream.data[3], (uint8_t) -91);
        ASSERT_EQ(stream.data[4], 47);
        ASSERT_EQ(stream.data[5], 245);
        ASSERT_EQ(stream.data[6], 101);
        ASSERT_EQ(stream.data[7], 16);
        ASSERT_EQ(stream.data[8], 78);
        ASSERT_EQ(stream.data[9], 20);

        uint8_t results[10] = { 0 };
        aout_stream_reset(&stream);

        res = aout_stream_read_u8(&stream, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[2]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[3]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[4]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[5]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[6]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[7]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[8]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[9]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u8(&stream, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

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

TEST stream_write_u16_then_read_u16(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_stream_write_u16(&stream, 21000);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, 1399);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, 2);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, -915);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, 47);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u16(&stream, 2339);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        uint16_t h0 = aout_hton_u16(21000);
        uint16_t h1 = aout_hton_u16(1399);
        uint16_t h2 = aout_hton_u16(2);
        uint16_t h3 = aout_hton_u16(-915);
        uint16_t h4 = aout_hton_u16(47);

        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h2)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h2)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h3)[0]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h3)[1]);
        ASSERT_EQ(stream.data[8], ((uint8_t*) &h4)[0]);
        ASSERT_EQ(stream.data[9], ((uint8_t*) &h4)[1]);

        uint16_t results[5] = { 0 };
        aout_stream_reset(&stream);

        res = aout_stream_read_u16(&stream, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u16(&stream, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u16(&stream, &results[2]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u16(&stream, &results[3]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u16(&stream, &results[4]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u16(&stream, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 10);

        ASSERT_EQ(results[0], 21000);
        ASSERT_EQ(results[1], 1399);
        ASSERT_EQ(results[2], 2);
        ASSERT_EQ(results[3], (uint16_t) -915);
        ASSERT_EQ(results[4], 47);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_u32_then_read_u32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_stream_write_u32(&stream, 210000);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u32(&stream, -915);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u32(&stream, 2339);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        uint32_t h0 = aout_hton_u32(210000);
        uint32_t h1 = aout_hton_u32(-915);

        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        uint32_t results[2] = { 0 };
        aout_stream_reset(&stream);

        res = aout_stream_read_u32(&stream, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u32(&stream, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u32(&stream, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        ASSERT_EQ(results[0], 210000);
        ASSERT_EQ(results[1], (uint32_t) -915);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_u64_then_read_u64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };

        res = aout_stream_write_u64(&stream, 21000000);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u64(&stream, -915);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_u64(&stream, 2339);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        uint64_t h0 = aout_hton_u64(21000000);
        uint64_t h1 = aout_hton_u64(-915);

        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(stream.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(stream.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(stream.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);

        uint64_t results[2] = { 0 };
        aout_stream_reset(&stream);

        res = aout_stream_read_u64(&stream, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u64(&stream, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_u64(&stream, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        ASSERT_EQ(results[0], 21000000);
        ASSERT_EQ(results[1], (uint64_t) -915);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_f32_then_read_f32(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };

        res = aout_stream_write_f32(&stream, 3.14159f);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_f32(&stream, -3.14159f);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_f32(&stream, 0.001f);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        uint32_t h0 = aout_hton_f32(3.14159f);
        uint32_t h1 = aout_hton_f32(-3.14159f);

        ASSERT_EQ(stream.data[0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        float32_t results[2] = { 0 };
        aout_stream_reset(&stream);

        res = aout_stream_read_f32(&stream, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_f32(&stream, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_f32(&stream, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 8);

        ASSERT_EQ(results[0], 3.14159f);
        ASSERT_EQ(results[1], -3.14159f);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_f64_then_read_f64(void) {
        uint8_t* data = calloc(19, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 19
        };

        aout_res res = { 0 };

        res = aout_stream_write_f64(&stream, 3.1415926535);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_f64(&stream, -3.1415926535);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_write_f64(&stream, 2339.0001);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        uint64_t h0 = aout_hton_f64(3.1415926535);
        uint64_t h1 = aout_hton_f64(-3.1415926535);

        ASSERT_EQ(stream.data[ 0], ((uint8_t*) &h0)[0]);
        ASSERT_EQ(stream.data[ 1], ((uint8_t*) &h0)[1]);
        ASSERT_EQ(stream.data[ 2], ((uint8_t*) &h0)[2]);
        ASSERT_EQ(stream.data[ 3], ((uint8_t*) &h0)[3]);
        ASSERT_EQ(stream.data[ 4], ((uint8_t*) &h0)[4]);
        ASSERT_EQ(stream.data[ 5], ((uint8_t*) &h0)[5]);
        ASSERT_EQ(stream.data[ 6], ((uint8_t*) &h0)[6]);
        ASSERT_EQ(stream.data[ 7], ((uint8_t*) &h0)[7]);
        ASSERT_EQ(stream.data[ 8], ((uint8_t*) &h1)[0]);
        ASSERT_EQ(stream.data[ 9], ((uint8_t*) &h1)[1]);
        ASSERT_EQ(stream.data[10], ((uint8_t*) &h1)[2]);
        ASSERT_EQ(stream.data[11], ((uint8_t*) &h1)[3]);
        ASSERT_EQ(stream.data[12], ((uint8_t*) &h1)[4]);
        ASSERT_EQ(stream.data[13], ((uint8_t*) &h1)[5]);
        ASSERT_EQ(stream.data[14], ((uint8_t*) &h1)[6]);
        ASSERT_EQ(stream.data[15], ((uint8_t*) &h1)[7]);
        ASSERT_EQ(stream.data[16], 0);
        ASSERT_EQ(stream.data[17], 0);
        ASSERT_EQ(stream.data[18], 0);

        float64_t results[2] = { 0 };
        aout_stream_reset(&stream);

        res = aout_stream_read_f64(&stream, &results[0]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_f64(&stream, &results[1]);
        ASSERT(AOUT_IS_OK(res));
        res = aout_stream_read_f64(&stream, &results[0]);
        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(aout_stream_get_count(&stream), 16);

        ASSERT_EQ(results[0], 3.1415926535);
        ASSERT_EQ(results[1], -3.1415926535);

        free(data);
        data = NULL;
        PASS();
}

SUITE(test_stream) {
        RUN_TEST(stream_write_u8);
        RUN_TEST(stream_write_u16);
        RUN_TEST(stream_write_u32);
        RUN_TEST(stream_write_u64);
        RUN_TEST(stream_write_f32);
        RUN_TEST(stream_write_f64);

        RUN_TEST(stream_read_u8);
        RUN_TEST(stream_read_u16);
        RUN_TEST(stream_read_u32);
        RUN_TEST(stream_read_u64);
        RUN_TEST(stream_read_f32);
        RUN_TEST(stream_read_f64);

        RUN_TEST(stream_write_u8_then_read_u8);
        RUN_TEST(stream_write_u16_then_read_u16);
        RUN_TEST(stream_write_u32_then_read_u32);
        RUN_TEST(stream_write_u64_then_read_u64);
        RUN_TEST(stream_write_f32_then_read_f32);
        RUN_TEST(stream_write_f64_then_read_f64);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_stream);

        GREATEST_MAIN_END();
}
