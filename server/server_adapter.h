#ifndef SERVER_SERVER_ADAPTER_H
#define SERVER_SERVER_ADAPTER_H

#include <common/connection.h>

typedef struct aout_server aout_server;
typedef struct aout_cl_msg_input aout_cl_msg_input;

typedef void (*aout_on_connection)(
                aout_server* server,
                aout_connection connection,
                void* context);

typedef void (*aout_on_disconnection)(
                aout_server* server,
                aout_connection connection,
                void* context);

// TODO: Make msg const?
typedef void (*aout_on_msg_input)(
                aout_server* server,
                aout_connection connection,
                aout_cl_msg_input* msg,
                void* context);

typedef struct aout_server_adapter {
        aout_on_connection on_connection;
        aout_on_disconnection on_disconnection;

        aout_on_msg_input on_msg_input;

        void* context;
} aout_server_adapter;

#endif
