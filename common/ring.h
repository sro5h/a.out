#ifndef COMMON_RING_H
#define COMMON_RING_H

#include "result.h"
#include "types.h"

typedef struct aout_ring {
        void* values;
        size_t value_size;
        size_t size;
        size_t capacity;
        size_t root;
} aout_ring;

COMMON_API aout_res aout_ring_init(
                aout_ring* self,
                size_t capacity,
                size_t value_size);

COMMON_API void aout_ring_fini(
                aout_ring* self);

COMMON_API aout_ring* aout_ring_create(
                size_t capacity,
                size_t value_size);

COMMON_API void aout_ring_destroy(
                aout_ring* self);

COMMON_API void aout_ring_clear(
                aout_ring* self);

COMMON_API void aout_ring_push_back(
                aout_ring* self,
                void const* value);

COMMON_API void aout_ring_push_front(
                aout_ring* self,
                void const* value);

COMMON_API void aout_ring_push(
                aout_ring* self,
                void const* value);

COMMON_API void aout_ring_pop_front(
                aout_ring* self);

COMMON_API void aout_ring_pop(
                aout_ring* self);

COMMON_API void* aout_ring_at(
                aout_ring const* self,
                size_t index);

COMMON_API void* aout_ring_rat(
                aout_ring const* self,
                size_t index);

COMMON_API void* aout_ring_front(
                aout_ring const* self);

COMMON_API void* aout_ring_back(
                aout_ring const* self);

COMMON_API size_t aout_ring_begin(
                aout_ring const* self);

COMMON_API size_t aout_ring_end(
                aout_ring const* self);

COMMON_API size_t aout_ring_size(
                aout_ring const* self);

COMMON_API bool aout_ring_empty(
                aout_ring const* self);

COMMON_API size_t aout_ring_index(
                aout_ring const* self,
                size_t index);

#endif
