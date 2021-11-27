#ifndef CLIENT_CLIENT_ADAPTER_H
#define CLIENT_CLIENT_ADAPTER_H

#include <common/connection.h>

typedef struct aout_client aout_client;
typedef struct aout_sv_msg_connection aout_sv_msg_connection;
typedef struct aout_sv_msg_state aout_sv_msg_state;

typedef void (*aout_on_connection)(
                aout_client* client,
                aout_connection* connection,
                void* context);

typedef void (*aout_on_disconnection)(
                aout_client* client,
                aout_connection* connection,
                void* context);

typedef void (*aout_on_msg_connection)(
                aout_client* client,
                aout_connection* connection,
                aout_sv_msg_connection* msg,
                void* context);

typedef void (*aout_on_msg_state)(
                aout_client* client,
                aout_connection* connection,
                aout_sv_msg_state* msg,
                void* context);

typedef struct aout_client_adapter {
        aout_on_connection on_connection;
        aout_on_disconnection on_disconnection;

        aout_on_msg_connection on_msg_connection;
        aout_on_msg_state on_msg_state;

        void* context;
} aout_client_adapter;

#endif
