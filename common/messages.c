#include "messages.h"
#include "stream.h"

aout_res aout_stream_write_cl_msg_input(
                aout_stream* self,
                aout_cl_msg_input* msg) {
        assert(self); assert(msg);

        if (!aout_stream_has_capacity(self, sizeof(*msg))) {
                return AOUT_ERR;
        }

        // Ignore return values, as aout_stream_has_capacity returned true
        aout_res res = { 0 };
        res = aout_stream_write_u64(self, msg->tick.value);
        assert(AOUT_IS_OK(res));

        for (size_t i = 0; i < AOUT_CL_MSG_INPUT_BUFFER_COUNT; ++i) {
                aout_input const* input = &msg->inputs[i];
                res = aout_stream_write_u8(self, input->right);
                assert(AOUT_IS_OK(res));
                res = aout_stream_write_u8(self, input->left);
                assert(AOUT_IS_OK(res));
                res = aout_stream_write_u8(self, input->up);
                assert(AOUT_IS_OK(res));
                res = aout_stream_write_u8(self, input->down);
                assert(AOUT_IS_OK(res));
        }

        return AOUT_OK;
}

aout_res aout_stream_write_sv_msg_connection(
                aout_stream* self,
                aout_sv_msg_connection* msg) {
        assert(self); assert(msg);

        // Make sure there is enough space to write each type on its own. This
        // leaves the option to optimize how many bytes are written.
        if (!aout_stream_has_capacity(self, sizeof(*msg))) {
                return AOUT_ERR;
        }

        // Ignore return values, as aout_stream_has_capacity returned true
        aout_res res = { 0 };
        res = aout_stream_write_u32(self, msg->id);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_u16(self, msg->peer_id);
        assert(AOUT_IS_OK(res));

        return AOUT_OK;
}

aout_res aout_stream_write_sv_msg_state(
                aout_stream* self,
                aout_sv_msg_state* msg) {
        assert(self); assert(msg);

        if (!aout_stream_has_capacity(self, sizeof(*msg))) {
                return AOUT_ERR;
        }

        aout_res res = { 0 };
        res = aout_stream_write_u64(self, msg->tick.value);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_f32(self, msg->state.p.x);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_f32(self, msg->state.p.y);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_f32(self, msg->state.v.x);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_f32(self, msg->state.v.y);
        assert(AOUT_IS_OK(res));

        return AOUT_OK;
}

aout_res aout_stream_read_cl_msg_input(
                aout_stream* self,
                aout_cl_msg_input* msg) {
        assert(self); assert(msg);

        aout_cl_msg_input tmp;

        if (AOUT_IS_ERR(aout_stream_read_u64(self, &tmp.tick.value))) {
                return AOUT_ERR;
        }

        for (size_t i = 0; i < AOUT_CL_MSG_INPUT_BUFFER_COUNT; ++i) {
                aout_input* input = &tmp.inputs[i];
                if (AOUT_IS_ERR(aout_stream_read_u8(self, &input->right))) {
                        return AOUT_ERR;
                }

                if (AOUT_IS_ERR(aout_stream_read_u8(self, &input->left))) {
                        return AOUT_ERR;
                }

                if (AOUT_IS_ERR(aout_stream_read_u8(self, &input->up))) {
                        return AOUT_ERR;
                }

                if (AOUT_IS_ERR(aout_stream_read_u8(self, &input->down))) {
                        return AOUT_ERR;
                }
        }

        *msg = tmp;
        return AOUT_OK;
}

aout_res aout_stream_read_sv_msg_connection(
                aout_stream* self,
                aout_sv_msg_connection* msg) {
        assert(self); assert(msg);

        aout_sv_msg_connection tmp;

        if (AOUT_IS_ERR(aout_stream_read_u32(self, &tmp.id))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_u16(self, &tmp.peer_id))) {
                return AOUT_ERR;
        }

        *msg = tmp;
        return AOUT_OK;
}

aout_res aout_stream_read_sv_msg_state(
                aout_stream* self,
                aout_sv_msg_state* msg) {
        assert(self); assert(msg);

        aout_sv_msg_state tmp;

        if (AOUT_IS_ERR(aout_stream_read_u64(self, &tmp.tick.value))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_f32(self, &tmp.state.p.x))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_f32(self, &tmp.state.p.y))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_f32(self, &tmp.state.v.x))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_f32(self, &tmp.state.v.y))) {
                return AOUT_ERR;
        }

        *msg = tmp;
        return AOUT_OK;
}
