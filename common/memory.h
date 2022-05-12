#include "types.h"

/**
  Try to allocate a zero initialised chunk of `size` bytes. Will call abort on
  failure.
 */
COMMON_API void* aout_acquire(
                size_t size);

/**
  Try to reallocate a chunk of `old_size` bytes. Will zero initialise any
  additional memory and call abort on failure.
 */
COMMON_API void* aout_realloc(
                void* ptr,
                size_t old_size,
                size_t new_size);

/**
  Release a previously allocated chunk of memory.
 */
void aout_release(
                void const* ptr);

/**
  Try to allocate a zero initialised chunk of `size` bytes. Will return NULL on
  failure.
 */
COMMON_API void* aout_try_acquire(
                size_t size);

/**
  Try to reallocate a chunk of `old_size` bytes. Will zero initialise any
  additional memory and return NULL on failure.

  Requirements:
    - `ptr` is not NULL
    - `old_size` > 0
    - `new_size` > 0

  Guarantees:
    - Return value is not NULL
 */
COMMON_API void* aout_try_realloc(
                void* ptr,
                size_t old_size,
                size_t new_size);
