#ifndef COMMON_PLATFORM_H
#define COMMON_PLATFORM_H

#include "types.h"

#define aout_abort_if(condition) \
        aout_abort_if_impl(condition, #condition, __FILE__, __LINE__)

COMMON_API void aout_abort(void);

COMMON_API void aout_abort_if_impl(
                bool condition,
                char const* condition_str,
                char const* file,
                size_t line);

#endif
