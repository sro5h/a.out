#include "id_pool.h"

#include <stdlib.h>
#include <string.h>

typedef struct aout_slot {
        uint8_t generation;
        bool in_use;
} aout_slot;

struct aout_id_pool {
        size_t size;
        size_t* unused_indices;
        size_t unused_indices_top;
        aout_slot* slots;
};

aout_id aout_id_make(
                size_t index,
                uint8_t generation) {
        assert(index <= AOUT_ID_POOL_MAX_INDEX);

        aout_id id = generation << AOUT_ID_POOL_GENERATION_SHIFT;
        id |= (index + 1) & AOUT_ID_POOL_INDEX_MASK;

        return id;
}

size_t aout_id_index(
                aout_id id) {
        assert(id != AOUT_ID_POOL_INVALID_ID);

        size_t index = id & AOUT_ID_POOL_INDEX_MASK;
        assert(index > 0);

        return index - 1;
}

uint8_t aout_id_generation(
                aout_id id) {
        assert(id != AOUT_ID_POOL_INVALID_ID);
        return (uint8_t) (id >> AOUT_ID_POOL_GENERATION_SHIFT);
}

aout_id_pool* aout_id_pool_create(
                size_t size) {
        assert(size <= AOUT_ID_POOL_MAX_SIZE);

        aout_id_pool* pool = calloc(1, sizeof(*pool));
        assert(pool);

        pool->size = size;
        pool->unused_indices_top = 0;

        // Slots must be zero
        pool->slots = calloc(pool->size, sizeof(*pool->slots));
        assert(pool->slots);

        // TODO: Maybe also change to calloc?
        pool->unused_indices = malloc(sizeof(size_t) * size);
        assert(pool->unused_indices);

        // Populate unused indices with [size - 1 .. 0]
        for (size_t i = 1; i <= pool->size; ++i) {
                pool->unused_indices[pool->unused_indices_top] = pool->size - i;
                ++pool->unused_indices_top;
        }

        return pool;
}

void aout_id_pool_destroy(
                aout_id_pool* pool) {
        if (pool) {
                free(pool->unused_indices);
                free(pool->slots);
                free(pool);
        }
}

aout_id aout_id_pool_id_create(
                aout_id_pool* pool) {
        assert(pool); assert(pool->unused_indices); assert(pool->slots);
        assert(pool->unused_indices_top <= pool->size);

        if (pool->unused_indices_top == 0) {
                return AOUT_ID_POOL_INVALID_ID;
        }

        --pool->unused_indices_top;
        size_t index = pool->unused_indices[pool->unused_indices_top];
        assert(index < pool->size);

        aout_slot* slot = &pool->slots[index];
        slot->in_use = true;
        return aout_id_make(index, slot->generation);
}

void aout_id_pool_id_destroy(
                aout_id_pool* pool,
                aout_id id) {
        assert(pool); assert(pool->unused_indices); assert(pool->slots);
        assert(id != AOUT_ID_POOL_INVALID_ID);
        assert(pool->unused_indices_top < pool->size);

        size_t index = aout_id_index(id);
        assert(index < pool->size);

        // Protect against double free
        // TODO: Only do this in debug mode
        assert(aout_id_pool_id_is_valid(pool, id));
        for (size_t i = 0; i < pool->unused_indices_top; ++i) {
                assert(pool->unused_indices[i] != index);
        }

        pool->unused_indices[pool->unused_indices_top] = index;
        ++pool->unused_indices_top;

        aout_slot* slot = &pool->slots[index];
        ++slot->generation;
        slot->in_use = false;
}

bool aout_id_pool_id_is_valid(
                aout_id_pool const* pool,
                aout_id id) {
        assert(pool); assert(pool->slots);

        if (id == AOUT_ID_POOL_INVALID_ID) {
                return false;
        }

        size_t index = aout_id_index(id);
        assert(index < pool->size);

        aout_slot* slot = &pool->slots[index];
        return slot->in_use && slot->generation == aout_id_generation(id);
}
