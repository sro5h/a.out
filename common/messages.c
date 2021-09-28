#include "messages.h"
#include "stream.h"

aout_res aout_stream_write_cl_msg_type(
                aout_stream* stream,
                aout_cl_msg_type type) {
        assert(stream);
        static_assert(sizeof(AOUT_TYPE_CL_MSG_TYPE) == sizeof(uint32_t));
        // TODO: Use AOUT_CL_MSG_TYPE_TYPE
        return aout_stream_write_u32(stream, (uint32_t) type);
}

aout_res aout_stream_write_sv_msg_type(
                aout_stream* stream,
                aout_sv_msg_type type) {
        assert(stream);
        static_assert(sizeof(AOUT_TYPE_SV_MSG_TYPE) == sizeof(uint32_t));
        // TODO: Use AOUT_SV_MSG_TYPE_TYPE
        return aout_stream_write_u32(stream, (uint32_t) type);
}

aout_res aout_stream_write_sv_msg_connection(
                aout_stream* stream,
                aout_sv_msg_connection* msg) {
        assert(stream); assert(msg);

        // TODO: Account for padding of aout_sv_msg_connection?
        if (!aout_stream_has_capacity(stream, sizeof(*msg))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        // Ignore return values, as aout_stream_has_capacity returned true
        aout_res res = { 0 };
        res = aout_stream_write_u32(stream, msg->id);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_u16(stream, msg->peer_id);
        assert(AOUT_IS_OK(res));

        return AOUT_OK;
}

aout_res aout_stream_read_cl_msg_type(
                aout_stream* stream,
                aout_cl_msg_type* type) {
        assert(stream); assert(type);
        static_assert(sizeof(AOUT_TYPE_CL_MSG_TYPE) == sizeof(uint32_t));

        // TODO: Use AOUT_CL_MSG_TYPE_TYPE
        uint32_t tmp;
        aout_res res = aout_stream_read_u32(stream, &tmp);

        if (AOUT_IS_OK(res)) {
                *type = (aout_cl_msg_type) tmp;
        }

        return res;
}

aout_res aout_stream_read_sv_msg_type(
                aout_stream* stream,
                aout_sv_msg_type* type) {
        assert(stream); assert(type);
        static_assert(sizeof(AOUT_TYPE_SV_MSG_TYPE) == sizeof(uint32_t));

        // TODO: Use AOUT_SV_MSG_TYPE_TYPE
        uint32_t tmp;
        aout_res res = aout_stream_read_u32(stream, &tmp);

        if (AOUT_IS_OK(res)) {
                *type = (aout_sv_msg_type) tmp;
        }

        return res;
}

aout_res aout_stream_read_sv_msg_connection(
                aout_stream* stream,
                aout_sv_msg_connection* msg) {
        assert(stream); assert(msg);

        if (!aout_stream_has_capacity(stream, sizeof(*msg))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        // Ignore return values, as aout_stream_has_capacity returned true
        aout_res res = { 0 };
        res = aout_stream_read_u32(stream, &msg->id);
        assert(AOUT_IS_OK(res));
        res = aout_stream_read_u16(stream, &msg->peer_id);
        assert(AOUT_IS_OK(res));

        return AOUT_OK;
}
