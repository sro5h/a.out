#include "messages.h"
#include "stream.h"

aout_res aout_stream_write_cl_msg_type(
                aout_stream* self,
                aout_cl_msg_type type) {
        assert(self);
        static_assert(sizeof(AOUT_TYPE_CL_MSG_TYPE) == sizeof(uint32_t));

        return aout_stream_write_u32(self, (uint32_t) type);
}

aout_res aout_stream_write_sv_msg_type(
                aout_stream* self,
                aout_sv_msg_type type) {
        assert(self);
        static_assert(sizeof(AOUT_TYPE_SV_MSG_TYPE) == sizeof(uint32_t));

        return aout_stream_write_u32(self, (uint32_t) type);
}

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
        res = aout_stream_write_u8(self, msg->input.right);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_u8(self, msg->input.left);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_u8(self, msg->input.up);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_u8(self, msg->input.down);
        assert(AOUT_IS_OK(res));

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
        res = aout_stream_write_f32(self, msg->position.x);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_f32(self, msg->position.y);
        assert(AOUT_IS_OK(res));

        return AOUT_OK;
}

aout_res aout_stream_read_cl_msg_type(
                aout_stream* self,
                aout_cl_msg_type* type) {
        assert(self); assert(type);
        static_assert(sizeof(AOUT_TYPE_CL_MSG_TYPE) == sizeof(uint32_t));

        uint32_t tmp;
        aout_res res = aout_stream_read_u32(self, &tmp);

        if (AOUT_IS_OK(res)) {
                *type = (aout_cl_msg_type) tmp;
        }

        return res;
}

aout_res aout_stream_read_sv_msg_type(
                aout_stream* self,
                aout_sv_msg_type* type) {
        assert(self); assert(type);
        static_assert(sizeof(AOUT_TYPE_SV_MSG_TYPE) == sizeof(uint32_t));

        uint32_t tmp;
        aout_res res = aout_stream_read_u32(self, &tmp);

        if (AOUT_IS_OK(res)) {
                *type = (aout_sv_msg_type) tmp;
        }

        return res;
}

aout_res aout_stream_read_cl_msg_input(
                aout_stream* self,
                aout_cl_msg_input* msg) {
        assert(self); assert(msg);

        aout_cl_msg_input tmp;

        if (AOUT_IS_ERR(aout_stream_read_u64(self, &tmp.tick.value))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_u8(self, &tmp.input.right))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_u8(self, &tmp.input.left))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_u8(self, &tmp.input.up))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_u8(self, &tmp.input.down))) {
                return AOUT_ERR;
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

        if (AOUT_IS_ERR(aout_stream_read_f32(self, &tmp.position.x))) {
                return AOUT_ERR;
        }

        if (AOUT_IS_ERR(aout_stream_read_f32(self, &tmp.position.y))) {
                return AOUT_ERR;
        }

        *msg = tmp;
        return AOUT_OK;
}
