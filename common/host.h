#ifndef COMMON_HOST_H
#define COMMON_HOST_H

#include "stream.h"

typedef struct _ENetHost ENetHost;

typedef uint16_t aout_msg_type;

typedef struct aout_msg_desc {
        uint16_t type;
        size_t size;
        uint32_t flags;
        void* value;
        aout_res (*write)(aout_stream*, void*);
} aout_msg_desc;

COMMON_API aout_res aout_host_send_msg(
                ENetHost* self,
                uint16_t peer_id,
                aout_msg_desc* msg_desc);

#endif
