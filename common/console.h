#ifndef COMMON_CONSOLE_H
#define COMMON_CONSOLE_H

#include "result.h"

typedef struct aout_sig_handler {
        void (*callback)(void*);
        void* context;
} aout_sig_handler;

COMMON_API aout_res aout_on_sigint(aout_sig_handler handler);

#endif
