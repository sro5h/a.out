#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <common/types.h>

typedef struct _ENetHost ENetHost;

// TODO: Move to common
typedef struct aout_connection {
        uint32_t id;
        uint16_t peer_id;
} aout_connection;

typedef struct aout_client {
        ENetHost* host;
        aout_connection connection;
        bool is_running;
} aout_client;

typedef enum aout_client_res {
        AOUT_CLIENT_OK,
        AOUT_CLIENT_ERR
} aout_client_res;

aout_client* aout_client_create(
                void);

void aout_client_destroy(
                aout_client* client);

void aout_client_update(
                aout_client* client);

aout_client_res aout_client_connect(
                aout_client* client,
                uint32_t ip,
                uint16_t port);

bool aout_client_is_running(
                aout_client* client);

#endif
