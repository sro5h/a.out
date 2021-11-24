#include "messages.h"
#include "stream.h"

#define AOUT_SV_MSG_CONNECTION_SIZE ((size_t) sizeof(uint32_t)\
                + sizeof(uint16_t))

static_assert(AOUT_SV_MSG_CONNECTION_SIZE <= sizeof(aout_sv_msg_connection));

aout_res aout_stream_write_cl_msg_type(
                aout_stream* stream,
                aout_cl_msg_type type) {
        assert(stream);
        static_assert(sizeof(AOUT_TYPE_CL_MSG_TYPE) == sizeof(uint32_t));

        return aout_stream_write_u32(stream, (uint32_t) type);
}

aout_res aout_stream_write_sv_msg_type(
                aout_stream* stream,
                aout_sv_msg_type type) {
        assert(stream);
        static_assert(sizeof(AOUT_TYPE_SV_MSG_TYPE) == sizeof(uint32_t));

        return aout_stream_write_u32(stream, (uint32_t) type);
}

aout_res aout_stream_write_sv_msg_connection(
                aout_stream* stream,
                aout_sv_msg_connection* msg) {
        assert(stream); assert(msg);

        // TODO: Try to remove this check in analogy to the read function
        // The problem is that one needs to know if there is enough space to
        // guarantee that the buffer won't be modified if the function fails
        // TODO: Maybe add transaction functionality to stream that guarantees
        // that the index gets reset if an error occurs. The buffer will be
        // modified but that shouldn't matter anyways?
        if (!aout_stream_has_capacity(stream, AOUT_SV_MSG_CONNECTION_SIZE)) {
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

aout_res aout_stream_write_sv_msg_state(
                aout_stream* stream,
                aout_sv_msg_state* msg) {
        assert(stream); assert(msg);

        // TODO: Either use this or the idea above.
        if (!aout_stream_has_capacity(stream, sizeof(*msg))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        aout_res res = { 0 };
        res = aout_stream_write_f32(stream, msg->position.x);
        assert(AOUT_IS_OK(res));
        res = aout_stream_write_f32(stream, msg->position.y);
        assert(AOUT_IS_OK(res));

        return AOUT_OK;
}

aout_res aout_stream_read_cl_msg_type(
                aout_stream* stream,
                aout_cl_msg_type* type) {
        assert(stream); assert(type);
        static_assert(sizeof(AOUT_TYPE_CL_MSG_TYPE) == sizeof(uint32_t));

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

        aout_sv_msg_connection tmp;

        if (AOUT_IS_ERR(aout_stream_read_u32(stream, &tmp.id))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        if (AOUT_IS_ERR(aout_stream_read_u16(stream, &tmp.peer_id))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        *msg = tmp;
        return AOUT_OK;
}

aout_res aout_stream_read_sv_msg_state(
                aout_stream* stream,
                aout_sv_msg_state* msg) {
        assert(stream); assert(msg);

        aout_sv_msg_state tmp;

        if (AOUT_IS_ERR(aout_stream_read_f32(stream, &tmp.position.x))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        if (AOUT_IS_ERR(aout_stream_read_f32(stream, &tmp.position.y))) {
                return AOUT_ERR(AOUT_STREAM_ERR_END_REACHED);
        }

        *msg = tmp;
        return AOUT_OK;
}
