#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <common/messages.h>
#include <common/result.h>

#define MAX_CONNECTIONS 10

typedef struct _ENetHost ENetHost;

// TODO: Maybe move to libcommon
typedef struct aout_connection {
        uint32_t id;
        uint16_t peer_id;
} aout_connection;

typedef struct aout_server {
        ENetHost* host;
        aout_connection connections[MAX_CONNECTIONS];
        bool is_running;
} aout_server;

typedef enum aout_server_res {
        AOUT_SERVER_OK,
        AOUT_SERVER_ERR,
} aout_server_res;

aout_server* aout_server_create(
                void);

void aout_server_destroy(
                aout_server* server);

void aout_server_update(
                aout_server* server);

aout_res aout_server_send_msg_connection(
                aout_server* server,
                uint16_t peer_id,
                aout_sv_msg_connection* msg);

bool aout_server_is_running(
                aout_server* server);

#endif
