#include <common/byte_order.h>
#include <common/messages.h>
#include <greatest/greatest.h>

static bool memval(
                void const* src,
                uint8_t value,
                size_t size);

TEST stream_write_sv_msg_join(void) {
        size_t const size = sizeof(aout_sv_msg_join) + 4;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        size_t cursor = 0;
        aout_sv_msg_join msg = {
                .id = 3290323,
                .peer_id = 23
        };
        uint32_t n_id = aout_hton_u32(msg.id);
        uint16_t n_peer_id = aout_hton_u16(msg.peer_id);

        res = aout_stream_write_sv_msg_join(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_id, sizeof(n_id)) == 0);
        cursor += sizeof(n_id);
        ASSERT(memcmp(stream.data + cursor, &n_peer_id, sizeof(n_peer_id)) == 0);
        cursor += sizeof(n_peer_id);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        res = aout_stream_write_sv_msg_join(&stream, &msg);
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
                .state = {
                        .p = { .x = 3.14159f, .y = -32.3299f },
                        .v = { .x = -255.32f, .y = 0.329191f }
                }
        };
        uint64_t n_tick = aout_hton_u64(msg.tick.value);
        uint32_t n_px = aout_hton_f32(msg.state.p.x);
        uint32_t n_py = aout_hton_f32(msg.state.p.y);
        uint32_t n_vx = aout_hton_f32(msg.state.v.x);
        uint32_t n_vy = aout_hton_f32(msg.state.v.y);

        res = aout_stream_write_sv_msg_state(&stream, &msg);
        cursor = 0;

        ASSERT(AOUT_IS_OK(res));
        ASSERT(memcmp(stream.data + cursor, &n_tick, sizeof(n_tick)) == 0);
        cursor += sizeof(n_tick);
        ASSERT(memcmp(stream.data + cursor, &n_px, sizeof(n_px)) == 0);
        cursor += sizeof(n_px);
        ASSERT(memcmp(stream.data + cursor, &n_py, sizeof(n_py)) == 0);
        cursor += sizeof(n_py);
        ASSERT(memcmp(stream.data + cursor, &n_vx, sizeof(n_vx)) == 0);
        cursor += sizeof(n_vx);
        ASSERT(memcmp(stream.data + cursor, &n_vy, sizeof(n_vy)) == 0);
        cursor += sizeof(n_vy);
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
        ASSERT(memcmp(stream.data + cursor, &n_vx, sizeof(n_vx)) == 0);
        cursor += sizeof(n_vx);
        ASSERT(memcmp(stream.data + cursor, &n_vy, sizeof(n_vy)) == 0);
        cursor += sizeof(n_vy);
        ASSERT(memval(stream.data + cursor, 0, size - cursor));

        free(data);
        data = NULL;
        PASS();
}

TEST stream_read_sv_msg_join(void) {
        uint8_t data[] = { 32, 129, -32, 239, 12, 4, 9, 94, 49, 3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_sv_msg_join msg = { 0 };
        uint32_t h_id = aout_ntoh_u32(*((uint32_t*) &data[0]));
        uint16_t h_peer_id = aout_ntoh_u16(*((uint16_t*) &data[4]));

        res = aout_stream_read_sv_msg_join(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(msg.id, h_id);
        ASSERT_EQ(msg.peer_id, h_peer_id);

        res = aout_stream_read_sv_msg_join(&stream, &msg);

        ASSERT(AOUT_IS_ERR(res));

        // Values should not have changed
        ASSERT_EQ(msg.id, h_id);
        ASSERT_EQ(msg.peer_id, h_peer_id);

        PASS();
}

TEST stream_read_sv_msg_state(void) {
        uint8_t data[] = { 54, 223, -20, 84, 59, 81, 10, 98, 32, 129, -32,
                           239, 12, 4, 9, 94, 49, 3, 17, 83, -124, 51, 98,
                           12, 27, -3 };
        aout_stream stream = {
                .data = data,
                .data_size = sizeof(data)
        };

        aout_res res = { 0 };
        aout_sv_msg_state msg = { 0 };
        uint64_t h_tick = aout_ntoh_u64(*((uint64_t*) &data[0]));
        float32_t h_px = aout_ntoh_f32(*((uint32_t*) &data[8]));
        float32_t h_py = aout_ntoh_f32(*((uint32_t*) &data[12]));
        float32_t h_vx = aout_ntoh_f32(*((uint32_t*) &data[16]));
        float32_t h_vy = aout_ntoh_f32(*((uint32_t*) &data[20]));

        res = aout_stream_read_sv_msg_state(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(msg.tick.value, h_tick);
        ASSERT_EQ(msg.state.p.x, h_px);
        ASSERT_EQ(msg.state.p.y, h_py);
        ASSERT_EQ(msg.state.v.x, h_vx);
        ASSERT_EQ(msg.state.v.y, h_vy);

        res = aout_stream_read_sv_msg_state(&stream, &msg);

        ASSERT(AOUT_IS_ERR(res));

        // Values should not have changed
        ASSERT_EQ(msg.tick.value, h_tick);
        ASSERT_EQ(msg.state.p.x, h_px);
        ASSERT_EQ(msg.state.p.y, h_py);
        ASSERT_EQ(msg.state.v.x, h_vx);
        ASSERT_EQ(msg.state.v.y, h_vy);

        PASS();
}

TEST stream_write_then_read_cl_msg_input(void) {
        size_t const size = sizeof(aout_cl_msg_input) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        aout_cl_msg_input msg = {
                .tick.value = 1337,
                .inputs = {
                        { true, false, true, false },
                        { true, true, true, false },
                        { false, true, false, false }
                }
        };

        res = aout_stream_write_cl_msg_input(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));

        aout_stream_reset(&stream);

        aout_cl_msg_input dst = { 0 };
        res = aout_stream_read_cl_msg_input(&stream, &dst);

        ASSERT(AOUT_IS_OK(res));
        ASSERT(aout_tick_cmp(msg.tick, dst.tick) == 0);
        for (size_t i = 0; i < AOUT_CL_MSG_INPUT_BUFFER_COUNT; ++i) {
                ASSERT_EQ(msg.inputs[i].right, dst.inputs[i].right);
                ASSERT_EQ(msg.inputs[i].left, dst.inputs[i].left);
                ASSERT_EQ(msg.inputs[i].up, dst.inputs[i].up);
                ASSERT_EQ(msg.inputs[i].down, dst.inputs[i].down);
        }

        res = aout_stream_read_cl_msg_input(&stream, &dst);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(aout_tick_cmp(msg.tick, dst.tick) == 0);
        for (size_t i = 0; i < AOUT_CL_MSG_INPUT_BUFFER_COUNT; ++i) {
                ASSERT_EQ(msg.inputs[i].right, dst.inputs[i].right);
                ASSERT_EQ(msg.inputs[i].left, dst.inputs[i].left);
                ASSERT_EQ(msg.inputs[i].up, dst.inputs[i].up);
                ASSERT_EQ(msg.inputs[i].down, dst.inputs[i].down);
        }

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_then_read_sv_msg_join(void) {
        size_t const size = sizeof(aout_sv_msg_join) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        aout_sv_msg_join msg = {
                .id = 1337,
                .peer_id = 42329,
        };

        res = aout_stream_write_sv_msg_join(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));

        aout_stream_reset(&stream);

        aout_sv_msg_join dst = { 0 };
        res = aout_stream_read_sv_msg_join(&stream, &dst);

        ASSERT(AOUT_IS_OK(res));
        ASSERT_EQ(msg.id, dst.id);
        ASSERT_EQ(msg.peer_id, dst.peer_id);

        res = aout_stream_read_sv_msg_join(&stream, &dst);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT_EQ(msg.id, dst.id);
        ASSERT_EQ(msg.peer_id, dst.peer_id);

        free(data);
        data = NULL;
        PASS();
}

TEST stream_write_then_read_sv_msg_state(void) {
        size_t const size = sizeof(aout_sv_msg_state) + 2;
        uint8_t* data = calloc(size, sizeof(*data));

        ASSERT(data);

        aout_stream stream = {
                .data = data,
                .data_size = size
        };

        aout_res res = { 0 };
        aout_sv_msg_state msg = {
                .tick.value = 3993,
                .state = {
                        .p = { .x = 3.14159f, .y = -32.3299f },
                        .v = { .x = -255.32f, .y = 0.329191f }
                }
        };

        res = aout_stream_write_sv_msg_state(&stream, &msg);

        ASSERT(AOUT_IS_OK(res));

        aout_stream_reset(&stream);

        aout_sv_msg_state dst = { 0 };
        res = aout_stream_read_sv_msg_state(&stream, &dst);

        ASSERT(AOUT_IS_OK(res));
        ASSERT(aout_tick_cmp(msg.tick, dst.tick) == 0);
        ASSERT_EQ(msg.state.p.x, dst.state.p.x);
        ASSERT_EQ(msg.state.p.y, dst.state.p.y);
        ASSERT_EQ(msg.state.v.x, dst.state.v.x);
        ASSERT_EQ(msg.state.v.y, dst.state.v.y);

        res = aout_stream_read_sv_msg_state(&stream, &dst);

        ASSERT(AOUT_IS_ERR(res));
        ASSERT(aout_tick_cmp(msg.tick, dst.tick) == 0);
        ASSERT_EQ(msg.state.p.x, dst.state.p.x);
        ASSERT_EQ(msg.state.p.y, dst.state.p.y);
        ASSERT_EQ(msg.state.v.x, dst.state.v.x);
        ASSERT_EQ(msg.state.v.y, dst.state.v.y);

        free(data);
        data = NULL;
        PASS();
}

SUITE(test_messages) {
        RUN_TEST(stream_write_sv_msg_join);
        RUN_TEST(stream_write_sv_msg_state);

        RUN_TEST(stream_read_sv_msg_join);
        RUN_TEST(stream_read_sv_msg_state);

        RUN_TEST(stream_write_then_read_cl_msg_input);
        RUN_TEST(stream_write_then_read_sv_msg_join);
        RUN_TEST(stream_write_then_read_sv_msg_state);
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
