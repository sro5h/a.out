#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "server_adapter.h"

#include <common/connection.h>
#include <common/messages.h>
#include <common/result.h>

typedef struct _ENetHost ENetHost;

// TODO: Rename to sv_endpoint
typedef struct aout_server {
        ENetHost* host;
        aout_connection* connections;
        aout_server_adapter adapter;
} aout_server;

void aout_server_ctor(
                aout_server* self,
                aout_server_adapter adapter,
                size_t connection_count);

void aout_server_dtor(
                aout_server* self);

aout_server* aout_server_new(
                aout_server_adapter adapter,
                size_t connection_count);

void aout_server_del(
                aout_server** out_self);

void aout_server_update(
                aout_server* self);

void aout_server_flush(
                aout_server* self);

// TODO: Rename to aout_server_queue_msg_*
aout_res aout_server_send_msg_join(
                aout_server* self,
                aout_sv_msg_join* msg);

aout_res aout_server_send_msg_state(
                aout_server* self,
                uint16_t peer_id,
                aout_sv_msg_state* msg);

#endif
