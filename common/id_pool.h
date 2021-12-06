#ifndef COMMON_ID_POOL_H
#define COMMON_ID_POOL_H

#include "types.h"

#define AOUT_ID_POOL_INVALID_ID        (0)
#define AOUT_ID_POOL_INDEX_MASK       (0x00ffffff)
#define AOUT_ID_POOL_GENERATION_SHIFT (24)
#define AOUT_ID_POOL_MAX_INDEX        (0xffffff - 1)
#define AOUT_ID_POOL_MAX_SIZE         (AOUT_ID_POOL_MAX_INDEX + 1)

typedef uint32_t aout_id;

aout_id aout_id_make(
                size_t index,
                uint8_t generation);

size_t aout_id_index(
                aout_id id);

uint8_t aout_id_generation(
                aout_id id);

typedef struct aout_id_pool aout_id_pool;

aout_id_pool* aout_id_pool_create(
                size_t size);

void aout_id_pool_destroy(
                aout_id_pool* pool);

aout_id aout_id_pool_id_create(
                aout_id_pool* pool);

void aout_id_pool_id_destroy(
                aout_id_pool* pool,
                aout_id id);

bool aout_id_pool_id_is_valid(
                aout_id_pool const* pool,
                aout_id id);

#endif
