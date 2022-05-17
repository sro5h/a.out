#ifndef COMMON_MESSAGES_H
#define COMMON_MESSAGES_H

#include "input.h"
#include "result.h"
#include "state.h"
#include "stream.h"
#include "tick.h"

typedef enum aout_cl_msg_type {
        AOUT_CL_MSG_TYPE_INPUT,
} aout_cl_msg_type;

typedef enum aout_sv_msg_type {
        AOUT_SV_MSG_TYPE_CONNECTION,
        AOUT_SV_MSG_TYPE_STATE,
} aout_sv_msg_type;

#define AOUT_CL_MSG_INPUT_BUFFER_COUNT (3)

typedef struct aout_cl_msg_input {
        aout_tick tick;
        aout_input inputs[AOUT_CL_MSG_INPUT_BUFFER_COUNT];
} aout_cl_msg_input;

// TODO: Rename to aout_sv_msg_join_self
typedef struct aout_sv_msg_connection {
        uint32_t id;
        uint16_t peer_id;
} aout_sv_msg_connection;

// TODO: Add aout_sv_msg_join

typedef struct aout_sv_msg_state {
        aout_tick tick;
        aout_state state;
} aout_sv_msg_state;

COMMON_API aout_res aout_stream_write_cl_msg_input(
                aout_stream* self,
                aout_cl_msg_input* msg);

COMMON_API aout_res aout_stream_write_sv_msg_connection(
                aout_stream* self,
                aout_sv_msg_connection* msg);

COMMON_API aout_res aout_stream_write_sv_msg_state(
                aout_stream* self,
                aout_sv_msg_state* msg);

COMMON_API aout_res aout_stream_read_cl_msg_input(
                aout_stream* self,
                aout_cl_msg_input* msg);

COMMON_API aout_res aout_stream_read_sv_msg_connection(
                aout_stream* self,
                aout_sv_msg_connection* msg);

COMMON_API aout_res aout_stream_read_sv_msg_state(
                aout_stream* self,
                aout_sv_msg_state* msg);

#endif
