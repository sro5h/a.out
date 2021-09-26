#include "console.h"

static aout_sig_handler priv_sig_int_handler;

// TODO: Make portable
//#if 1
#include <signal.h>
#include <unistd.h>

static void priv_sa_handler(int signal) {
        (void) signal;
        priv_sig_int_handler.callback(priv_sig_int_handler.context);
}

aout_res aout_on_sig_int(
                aout_sig_handler handler) {
        priv_sig_int_handler = handler;

        struct sigaction sig_action;
        sig_action.sa_handler = priv_sa_handler;
        sig_action.sa_flags = 0;

        if (sigemptyset(&sig_action.sa_mask) < 0) {
                return AOUT_ERR(AOUT_SIG_ERR);
        }

        if (sigaction(SIGINT, &sig_action, NULL) < 0) {
                return AOUT_ERR(AOUT_SIG_ERR);
        }

        return AOUT_OK;
}
//#else
//#       error unsupported platform
//#endif
