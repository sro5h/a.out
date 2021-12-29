#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include "server_adapter.h"

#include <common/connection.h>
#include <common/messages.h>
#include <common/result.h>

typedef struct _ENetHost ENetHost;

typedef struct aout_server {
        ENetHost* host;
        aout_connection* connections;
        aout_server_adapter adapter;
} aout_server;

typedef enum aout_server_res {
        AOUT_SERVER_OK,
        AOUT_SERVER_ERR,
} aout_server_res;

aout_server* aout_server_create(
                aout_server_adapter adapter,
                size_t connection_count);

void aout_server_destroy(
                aout_server* self);

void aout_server_update(
                aout_server* self);

aout_res aout_server_send_msg_connection(
                aout_server* self,
                uint16_t peer_id,
                aout_sv_msg_connection* msg);

aout_res aout_server_send_msg_state(
                aout_server* self,
                uint16_t peer_id,
                aout_sv_msg_state* msg);

#endif
