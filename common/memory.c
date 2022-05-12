#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* aout_acquire(
                size_t size) {
        void* result = aout_try_acquire(size);

        if (!result) {
                fprintf(stderr, "FATAL: aout_acquire returned NULL\n");
                abort();
        }

        return result;
}

void* aout_realloc(
                void* ptr,
                size_t old_size,
                size_t new_size) {
        void* result = aout_try_realloc(ptr, old_size, new_size);

        if (!result) {
                fprintf(stderr, "FATAL: aout_realloc returned NULL\n");
                abort();
        }

        return result;
}

void aout_release(
                void const* ptr) {
        free((void*) ptr);
}

void* aout_try_acquire(
                size_t size) {
        return calloc(1, size);
}

void* aout_try_realloc(
                void* ptr,
                size_t old_size,
                size_t new_size) {
        assert(ptr); assert(old_size > 0); assert(new_size > 0);

        if (new_size <= old_size) {
                return ptr;
        }

        void* new_ptr = aout_try_acquire(new_size);
        if (!new_ptr) {
                return NULL;
        }

        memcpy(new_ptr, ptr, old_size);
        aout_release(ptr);

        return new_ptr;
}
