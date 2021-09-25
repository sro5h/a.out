#ifndef COMMON_CONSOLE_H
#define COMMON_CONSOLE_H

#include "result.h"

typedef struct aout_sig_handler {
        void (*callback)(void*);
        void* context;
} aout_sig_handler;

typedef enum aout_sig_res {
        AOUT_SIG_OK,
        AOUT_SIG_ERR
} aout_sig_res;

aout_res COMMON_API aout_on_sig_int(aout_sig_handler handler);

#endif
