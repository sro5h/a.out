#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "client_adapter.h"

#include <common/connection.h>
#include <common/messages.h>
#include <common/result.h>

typedef struct _ENetHost ENetHost;
typedef struct _ENetPeer ENetPeer;

typedef struct aout_client {
        ENetHost* host;
        ENetPeer* peer;
        aout_connection connection;
        aout_client_adapter adapter;
} aout_client;

void aout_client_ctor(
                aout_client* self,
                aout_client_adapter adapter);

void aout_client_dtor(
                aout_client* self);

aout_client* aout_client_new(
                aout_client_adapter adapter);

void aout_client_del(
                aout_client** out_self);

void aout_client_update(
                aout_client* self);

void aout_client_flush(
                aout_client* self);

aout_res aout_client_connect(
                aout_client* self,
                uint32_t ip,
                uint16_t port);

aout_res aout_client_send_msg_input(
                aout_client* self,
                aout_cl_msg_input* msg);

aout_connection aout_client_get_connection(
                aout_client const* self);

#endif
