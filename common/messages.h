#ifndef COMMON_MESSAGES_H
#define COMMON_MESSAGES_H

#include "result.h"
#include "stream.h"
#include "vector2.h"

#define AOUT_TYPE_CL_MSG_TYPE uint32_t

typedef enum aout_cl_msg_type {
        AOUT_CL_MSG_TYPE_INPUT,
} aout_cl_msg_type;

#define AOUT_TYPE_SV_MSG_TYPE uint32_t

typedef enum aout_sv_msg_type {
        AOUT_SV_MSG_TYPE_CONNECTION,
        AOUT_SV_MSG_TYPE_STATE,
} aout_sv_msg_type;

// TODO: Rename to *_command
typedef struct aout_cl_msg_input {
        uint8_t up, down, left, right;
} aout_cl_msg_input;

typedef struct aout_sv_msg_connection {
        uint32_t id;
        uint16_t peer_id;
} aout_sv_msg_connection;

typedef struct aout_sv_msg_state {
        aout_vec2 position;
} aout_sv_msg_state;

COMMON_API aout_res aout_stream_write_cl_msg_type(
                aout_stream* stream,
                aout_cl_msg_type type);

COMMON_API aout_res aout_stream_write_sv_msg_type(
                aout_stream* stream,
                aout_sv_msg_type type);

COMMON_API aout_res aout_stream_write_cl_msg_input(
                aout_stream* stream,
                aout_cl_msg_input* msg);

COMMON_API aout_res aout_stream_write_sv_msg_connection(
                aout_stream* stream,
                aout_sv_msg_connection* msg);

COMMON_API aout_res aout_stream_write_sv_msg_state(
                aout_stream* stream,
                aout_sv_msg_state* msg);

COMMON_API aout_res aout_stream_read_cl_msg_type(
                aout_stream* stream,
                aout_cl_msg_type* type);

COMMON_API aout_res aout_stream_read_sv_msg_type(
                aout_stream* stream,
                aout_sv_msg_type* type);

COMMON_API aout_res aout_stream_read_cl_msg_input(
                aout_stream* stream,
                aout_cl_msg_input* msg);

COMMON_API aout_res aout_stream_read_sv_msg_connection(
                aout_stream* stream,
                aout_sv_msg_connection* msg);

COMMON_API aout_res aout_stream_read_sv_msg_state(
                aout_stream* stream,
                aout_sv_msg_state* msg);

#endif
