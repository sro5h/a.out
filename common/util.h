#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include "types.h"

typedef struct cpSpace cpSpace;
typedef struct cpBody cpBody;

COMMON_API void aout_print_bytes(
                uint8_t* bytes,
                size_t size);

// Safe to use even if cpSpaceStep will not be called again because
// cpSpaceRemove will also call all post step callbacks.
COMMON_API void aout_space_free_children_post_step(
                cpSpace* space);

COMMON_API void aout_space_free(
                cpSpace* space);

COMMON_API void aout_body_free_children(
                cpBody* body);

COMMON_API void aout_body_free(
                cpBody* body);

#endif
