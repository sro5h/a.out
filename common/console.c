#include "console.h"

static aout_sig_handler priv_sigint_handler;

// TODO: Make portable
//#if 1
#include <signal.h>
#include <unistd.h>

static void priv_sa_handler(int signal) {
        (void) signal;
        priv_sigint_handler.callback(priv_sigint_handler.context);
}

// TODO: Remove sigaction if NULL is passed
aout_res aout_on_sigint(
                aout_sig_handler handler) {
        priv_sigint_handler = handler;

        struct sigaction sig_action;
        sig_action.sa_handler = priv_sa_handler;
        sig_action.sa_flags = 0;

        if (sigemptyset(&sig_action.sa_mask) < 0) {
                return AOUT_ERR;
        }

        if (sigaction(SIGINT, &sig_action, NULL) < 0) {
                return AOUT_ERR;
        }

        return AOUT_OK;
}
//#else
//#       error unsupported platform
//#endif
