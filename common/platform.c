#include "platform.h"
#include "log.h"

#include <stdlib.h>

void aout_abort(void) {
        abort();
}

void aout_abort_if_impl(
                bool condition,
                char const* condition_str,
                char const* file,
                size_t line) {
        if (!condition) {
                return;
        }

        log_log(LOG_FATAL, file, line, "assertion '%s' failed", condition_str);
        aout_abort();
}
