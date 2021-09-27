#include <common/byte_order.h>
#include <common/messages.h>
#include <greatest/greatest.h>

TEST stream_write_cl_msg_type(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        aout_cl_msg_type type = AOUT_CL_MSG_TYPE_INPUT;
        uint32_t n_type = aout_hton_u32((uint32_t) type);

        res = aout_stream_write_cl_msg_type(&stream, type);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_type)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_type)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_type)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_type)[3]);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        res = aout_stream_write_cl_msg_type(&stream, type);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_type)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_type)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_type)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_type)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &n_type)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &n_type)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &n_type)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &n_type)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        res = aout_stream_write_cl_msg_type(&stream, type);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_type)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_type)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_type)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_type)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &n_type)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &n_type)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &n_type)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &n_type)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_sv_msg_type(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        aout_sv_msg_type type0 = AOUT_SV_MSG_TYPE_CONNECTION;
        uint32_t n_type0 = aout_hton_u32((uint32_t) type0);

        res = aout_stream_write_sv_msg_type(&stream, type0);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_type0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_type0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_type0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_type0)[3]);
        ASSERT_EQ(stream.data[4], 0);
        ASSERT_EQ(stream.data[5], 0);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        aout_sv_msg_type type1 = AOUT_SV_MSG_TYPE_STATE;
        uint32_t n_type1 = aout_hton_u32((uint32_t) type1);

        res = aout_stream_write_sv_msg_type(&stream, type1);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_type0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_type0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_type0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_type0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &n_type1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &n_type1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &n_type1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &n_type1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        res = aout_stream_write_sv_msg_type(&stream, type1);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_type0)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_type0)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_type0)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_type0)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &n_type1)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &n_type1)[1]);
        ASSERT_EQ(stream.data[6], ((uint8_t*) &n_type1)[2]);
        ASSERT_EQ(stream.data[7], ((uint8_t*) &n_type1)[3]);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_sv_msg_connection(void) {
        uint8_t* data = calloc(10, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = 10
        };

        aout_res res = { 0 };
        aout_sv_msg_connection msg = {
                .id = 3290323,
                .peer_id = 23
        };
        uint32_t n_id = aout_hton_u32(msg.id);
        uint16_t n_peer_id = aout_hton_u16(msg.peer_id);

        res = aout_stream_write_sv_msg_connection(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_id)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_id)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_id)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_id)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &n_peer_id)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &n_peer_id)[1]);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        res = aout_stream_write_sv_msg_connection(&stream, &msg);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);
        ASSERT_EQ(stream.data[0], ((uint8_t*) &n_id)[0]);
        ASSERT_EQ(stream.data[1], ((uint8_t*) &n_id)[1]);
        ASSERT_EQ(stream.data[2], ((uint8_t*) &n_id)[2]);
        ASSERT_EQ(stream.data[3], ((uint8_t*) &n_id)[3]);
        ASSERT_EQ(stream.data[4], ((uint8_t*) &n_peer_id)[0]);
        ASSERT_EQ(stream.data[5], ((uint8_t*) &n_peer_id)[1]);
        ASSERT_EQ(stream.data[6], 0);
        ASSERT_EQ(stream.data[7], 0);
        ASSERT_EQ(stream.data[8], 0);
        ASSERT_EQ(stream.data[9], 0);

        free(data);
        data = NULL;
        PASS();
}

// Depends on enum order of aout_sv_msg_type!
TEST stream_read_cl_msg_type(void) {
        uint8_t data[] = { 0, 0, 0, 0, 0, 0, 0, 0, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_cl_msg_type types[2] = { 0 };

        res = aout_stream_read_cl_msg_type(&stream, &types[0]);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(types[0], AOUT_CL_MSG_TYPE_INPUT);

        res = aout_stream_read_cl_msg_type(&stream, &types[1]);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(types[1], AOUT_CL_MSG_TYPE_INPUT);

        res = aout_stream_read_cl_msg_type(&stream, &types[0]);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(types[0], AOUT_CL_MSG_TYPE_INPUT);

        PASS();
}

// Depends on enum order of aout_sv_msg_type!
TEST stream_read_sv_msg_type(void) {
        uint8_t data[] = { 0, 0, 0, 0, 0, 0, 0, 1, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_sv_msg_type types[2] = { 0 };

        res = aout_stream_read_sv_msg_type(&stream, &types[0]);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(types[0], AOUT_SV_MSG_TYPE_CONNECTION);

        res = aout_stream_read_sv_msg_type(&stream, &types[1]);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(types[1], AOUT_SV_MSG_TYPE_STATE);

        res = aout_stream_read_sv_msg_type(&stream, &types[0]);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(types[0], AOUT_SV_MSG_TYPE_CONNECTION);

        PASS();
}

TEST stream_read_sv_msg_connection(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_sv_msg_connection msg = { 0 };
        uint32_t h_id = aout_ntoh_u32(*((uint32_t*) &data[0]));
        uint16_t h_peer_id = aout_ntoh_u16(*((uint16_t*) &data[4]));

        res = aout_stream_read_sv_msg_connection(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(msg.id, h_id);
        ASSERT_EQ(msg.peer_id, h_peer_id);

        res = aout_stream_read_sv_msg_connection(&stream, &msg);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(res.code, AOUT_STREAM_ERR_END_REACHED);

        // Values should not have changed
        ASSERT_EQ(msg.id, h_id);
        ASSERT_EQ(msg.peer_id, h_peer_id);

        PASS();
}

SUITE(test_messages) {
        RUN_TEST(stream_write_cl_msg_type);
        RUN_TEST(stream_write_sv_msg_type);
        RUN_TEST(stream_write_sv_msg_connection);

        RUN_TEST(stream_read_cl_msg_type);
        RUN_TEST(stream_read_sv_msg_type);
        RUN_TEST(stream_read_sv_msg_connection);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_messages);

        GREATEST_MAIN_END();
}
