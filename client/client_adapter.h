#ifndef CLIENT_CLIENT_ADAPTER_H
#define CLIENT_CLIENT_ADAPTER_H

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

typedef struct client_adapter {
        aout_on_connection     on_connection;
        aout_on_disconnection  on_disconnection;
        aout_on_msg_connection on_msg_connection;
        void* context; // Maybe move to client? Rather not, is coupled with functions
} client_adapter;

#endif
