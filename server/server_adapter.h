#ifndef SERVER_SERVER_ADAPTER_H
#define SERVER_SERVER_ADAPTER_H

#include <common/connection.h>

typedef struct aout_server aout_server;

typedef void (*aout_on_connection)(
                aout_server* server,
                aout_connection* connection,
                void* context);

typedef void (*aout_on_disconnection)(
                aout_server* server,
                aout_connection* connection,
                void* context);

typedef struct aout_server_adapter {
        aout_on_connection    on_connection;
        aout_on_disconnection on_disconnection;

        void* context;
} aout_server_adapter;

#endif
