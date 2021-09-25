#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <common/types.h>

#define MAX_CONNECTIONS 10

typedef struct _ENetHost ENetHost;

typedef struct aout_connection {
        uint32_t id;
        uint16_t peer_id;
} aout_connection;

typedef struct aout_server {
        ENetHost* host;
        aout_connection connections[MAX_CONNECTIONS];
        bool is_running;
} aout_server;

aout_server* aout_server_create(
                void);

void aout_server_destroy(
                aout_server* server);

void aout_server_update(
                aout_server* server);

bool aout_server_is_running(
                aout_server* server);

#endif
