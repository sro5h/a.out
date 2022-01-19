#include <common/byte_order.h>
#include <common/messages.h>
#include <greatest/greatest.h>

static bool memval(
                void const* src,
                uint8_t value,
                size_t size);

TEST stream_write_cl_msg_type(void) {
        size_t const size = 2 * sizeof(aout_cl_msg_type) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_cl_msg_type type = AOUT_CL_MSG_TYPE_INPUT;
        uint32_t n_type = aout_hton_u32((uint32_t) type);

        res = aout_stream_write_cl_msg_type(&stream, type);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_cl_msg_type(&stream, type);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_cl_msg_type(&stream, type);
        cursor = 0;

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_sv_msg_type(void) {
        size_t const size = 2 * sizeof(aout_sv_msg_type) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_sv_msg_type type0 = AOUT_SV_MSG_TYPE_CONNECTION;
        uint32_t n_type0 = aout_hton_u32((uint32_t) type0);

        res = aout_stream_write_sv_msg_type(&stream, type0);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_type0, sizeof(n_type0)) == 0);
        cursor += sizeof(n_type0);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        aout_sv_msg_type type1 = AOUT_SV_MSG_TYPE_STATE;
        uint32_t n_type1 = aout_hton_u32((uint32_t) type1);

        res = aout_stream_write_sv_msg_type(&stream, type1);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_type0, sizeof(n_type0)) == 0);
        cursor += sizeof(n_type0);
        ASSERT(memcmp(stream.data + cursor, &n_type1, sizeof(n_type1)) == 0);
        cursor += sizeof(n_type1);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_sv_msg_type(&stream, type1);
        cursor = 0;

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(memcmp(stream.data + cursor, &n_type0, sizeof(n_type0)) == 0);
        cursor += sizeof(n_type0);
        ASSERT(memcmp(stream.data + cursor, &n_type1, sizeof(n_type1)) == 0);
        cursor += sizeof(n_type1);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_cl_msg_input(void) {
        size_t const size = sizeof(aout_cl_msg_input) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_cl_msg_input msg = {
                .tick.value = 1337,
                .input = {
                        .right = false,
                        .left = true,
                        .up = false,
                        .down = true
                }
        };
        uint64_t n_tick = aout_hton_u64(msg.tick.value);
        uint8_t n_right = msg.input.right;
        uint8_t n_left = msg.input.left;
        uint8_t n_up = msg.input.up;
        uint8_t n_down = msg.input.down;

        res = aout_stream_write_cl_msg_input(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_tick, sizeof(n_tick)) == 0);
        cursor += sizeof(n_tick);
        ASSERT(memcmp(stream.data + cursor, &n_right, sizeof(n_right)) == 0);
        cursor += sizeof(n_right);
        ASSERT(memcmp(stream.data + cursor, &n_left, sizeof(n_left)) == 0);
        cursor += sizeof(n_left);
        ASSERT(memcmp(stream.data + cursor, &n_up, sizeof(n_up)) == 0);
        cursor += sizeof(n_up);
        ASSERT(memcmp(stream.data + cursor, &n_down, sizeof(n_down)) == 0);
        cursor += sizeof(n_down);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_cl_msg_input(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(memcmp(stream.data + cursor, &n_tick, sizeof(n_tick)) == 0);
        cursor += sizeof(n_tick);
        ASSERT(memcmp(stream.data + cursor, &n_right, sizeof(n_right)) == 0);
        cursor += sizeof(n_right);
        ASSERT(memcmp(stream.data + cursor, &n_left, sizeof(n_left)) == 0);
        cursor += sizeof(n_left);
        ASSERT(memcmp(stream.data + cursor, &n_up, sizeof(n_up)) == 0);
        cursor += sizeof(n_up);
        ASSERT(memcmp(stream.data + cursor, &n_down, sizeof(n_down)) == 0);
        cursor += sizeof(n_down);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_sv_msg_connection(void) {
        size_t const size = sizeof(aout_sv_msg_connection) + 4;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_sv_msg_connection msg = {
                .id = 3290323,
                .peer_id = 23
        };
        uint32_t n_id = aout_hton_u32(msg.id);
        uint16_t n_peer_id = aout_hton_u16(msg.peer_id);

        res = aout_stream_write_sv_msg_connection(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_id, sizeof(n_id)) == 0);
        cursor += sizeof(n_id);
        ASSERT(memcmp(stream.data + cursor, &n_peer_id, sizeof(n_peer_id)) == 0);
        cursor += sizeof(n_peer_id);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_sv_msg_connection(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(memcmp(stream.data + cursor, &n_id, sizeof(n_id)) == 0);
        cursor += sizeof(n_id);
        ASSERT(memcmp(stream.data + cursor, &n_peer_id, sizeof(n_peer_id)) == 0);
        cursor += sizeof(n_peer_id);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_sv_msg_state(void) {
        size_t const size = sizeof(aout_sv_msg_state) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_sv_msg_state msg = {
                .tick.value = 3993,
                .position = { .x = 3.14159f, .y = -32.3299f }
        };
        uint64_t n_tick = aout_hton_u64(msg.tick.value);
        uint32_t n_px = aout_hton_f32(msg.position.x);
        uint32_t n_py = aout_hton_f32(msg.position.y);

        res = aout_stream_write_sv_msg_state(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_tick, sizeof(n_tick)) == 0);
        cursor += sizeof(n_tick);
        ASSERT(memcmp(stream.data + cursor, &n_px, sizeof(n_px)) == 0);
        cursor += sizeof(n_px);
        ASSERT(memcmp(stream.data + cursor, &n_py, sizeof(n_py)) == 0);
        cursor += sizeof(n_py);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_sv_msg_state(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(memcmp(stream.data + cursor, &n_tick, sizeof(n_tick)) == 0);
        cursor += sizeof(n_tick);
        ASSERT(memcmp(stream.data + cursor, &n_px, sizeof(n_px)) == 0);
        cursor += sizeof(n_px);
        ASSERT(memcmp(stream.data + cursor, &n_py, sizeof(n_py)) == 0);
        cursor += sizeof(n_py);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

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

TEST stream_read_cl_msg_input(void) {
        uint8_t data[] = { 7, 32, -1, 39, 80, 21, 48, 79, 32, 129, -32, 239, 12, 4 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_cl_msg_input msg = { 0 };
        uint64_t h_tick = aout_ntoh_u64(*((uint64_t*) &data[0]));
        uint8_t h_right = data[8];
        uint8_t h_left = data[9];
        uint8_t h_up = data[10];
        uint8_t h_down = data[11];

        res = aout_stream_read_cl_msg_input(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(msg.tick.value, h_tick);
        ASSERT_EQ(msg.input.right, h_right);
        ASSERT_EQ(msg.input.left, h_left);
        ASSERT_EQ(msg.input.up, h_up);
        ASSERT_EQ(msg.input.down, h_down);

        res = aout_stream_read_cl_msg_input(&stream, &msg);
        ASSERT(AOUT_IS_ERR(res));

        // Values should not have changed
        ASSERT_EQ(msg.tick.value, h_tick);
        ASSERT_EQ(msg.input.right, h_right);
        ASSERT_EQ(msg.input.left, h_left);
        ASSERT_EQ(msg.input.up, h_up);
        ASSERT_EQ(msg.input.down, h_down);

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

        // Values should not have changed
        ASSERT_EQ(msg.id, h_id);
        ASSERT_EQ(msg.peer_id, h_peer_id);

        PASS();
}

TEST stream_read_sv_msg_state(void) {
        uint8_t data[] = { 54, 223, -20, 84, 59, 81, 10, 98, 32, 129, -32,
                           239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_sv_msg_state msg = { 0 };
        uint64_t h_tick = aout_ntoh_u64(*((uint64_t*) &data[0]));
        float32_t h_position_x = aout_ntoh_f32(*((uint32_t*) &data[8]));
        float32_t h_position_y = aout_ntoh_f32(*((uint32_t*) &data[12]));

        res = aout_stream_read_sv_msg_state(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(msg.tick.value, h_tick);
        ASSERT_EQ(msg.position.x, h_position_x);
        ASSERT_EQ(msg.position.y, h_position_y);

        res = aout_stream_read_sv_msg_state(&stream, &msg);

        ASSERT(AOUT_IS_ERR(res));

        // Values should not have changed
        ASSERT_EQ(msg.tick.value, h_tick);
        ASSERT_EQ(msg.position.x, h_position_x);
        ASSERT_EQ(msg.position.y, h_position_y);

        PASS();
}

TEST stream_write_cl_msg_type_then_read_cl_msg_type(void) {
        size_t const size = sizeof(aout_cl_msg_type) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_cl_msg_type type = AOUT_CL_MSG_TYPE_INPUT;
        uint32_t n_type = aout_hton_u32((uint32_t) type);

        res = aout_stream_write_cl_msg_type(&stream, type);

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        aout_cl_msg_type result = AOUT_CL_MSG_TYPE_INPUT;
        aout_stream_reset(&stream);

        res = aout_stream_read_cl_msg_type(&stream, &result);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(result, type);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_sv_msg_type_then_read_sv_msg_type(void) {
        size_t const size = sizeof(aout_sv_msg_type) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_sv_msg_type type = AOUT_SV_MSG_TYPE_CONNECTION;
        uint32_t n_type = aout_hton_u32((uint32_t) type);

        res = aout_stream_write_sv_msg_type(&stream, type);

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_type, sizeof(n_type)) == 0);
        cursor += sizeof(n_type);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        aout_sv_msg_type result = AOUT_SV_MSG_TYPE_STATE;
        aout_stream_reset(&stream);

        res = aout_stream_read_sv_msg_type(&stream, &result);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(result, type);

        free(data);
        data = NULL;
        PASS();
}

SUITE(test_messages) {
        RUN_TEST(stream_write_cl_msg_type);
        RUN_TEST(stream_write_sv_msg_type);
        RUN_TEST(stream_write_cl_msg_input);
        RUN_TEST(stream_write_sv_msg_connection);
        RUN_TEST(stream_write_sv_msg_state);

        RUN_TEST(stream_read_cl_msg_type);
        RUN_TEST(stream_read_sv_msg_type);
        RUN_TEST(stream_read_cl_msg_input);
        RUN_TEST(stream_read_sv_msg_connection);
        RUN_TEST(stream_read_sv_msg_state);

        RUN_TEST(stream_write_cl_msg_type_then_read_cl_msg_type);
        RUN_TEST(stream_write_sv_msg_type_then_read_sv_msg_type);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_messages);

        GREATEST_MAIN_END();
}

static bool memval(
                void const* src,
                uint8_t value,
                size_t size) {
        unsigned char const* bytes = src;

        for (size_t i = 0; i < size; ++i) {
                if (bytes[i] != (unsigned char) value) {
                        return false;
                }
        }

        return true;
}
