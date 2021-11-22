#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include "types.h"

typedef struct cpSpace cpSpace;

COMMON_API void aout_print_bytes(
                uint8_t* bytes,
                size_t size);

COMMON_API void aout_space_free_children(
                cpSpace* space);

#endif
