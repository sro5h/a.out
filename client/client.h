#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "client_adapter.h"

#include <common/connection.h>
#include <common/messages.h>
#include <common/result.h>

typedef struct _ENetHost ENetHost;

typedef struct aout_client {
        ENetHost* host;
        aout_connection connection;
        aout_client_adapter adapter;
} aout_client;

typedef enum aout_client_res {
        AOUT_CLIENT_OK,
        AOUT_CLIENT_ERR
} aout_client_res;

aout_client* aout_client_create(
                aout_client_adapter adapter);

void aout_client_destroy(
                aout_client* client);

void aout_client_update(
                aout_client* client);

aout_res aout_client_connect(
                aout_client* client,
                uint32_t ip,
                uint16_t port);

aout_connection aout_client_get_connection(
                aout_client const* client);

#endif
