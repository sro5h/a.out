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

        aout_id_pool* self = calloc(1, sizeof(*self));
        assert(self);

        self->size = size;
        self->unused_indices_top = 0;

        // Slots must be zero
        self->slots = calloc(self->size, sizeof(*self->slots));
        assert(self->slots);

        self->unused_indices = calloc(self->size, sizeof(*self->unused_indices));
        assert(self->unused_indices);

        // Populate unused indices with [size - 1 .. 0]
        for (size_t i = 1; i <= self->size; ++i) {
                self->unused_indices[self->unused_indices_top] = self->size - i;
                ++self->unused_indices_top;
        }

        return self;
}

void aout_id_pool_destroy(
                aout_id_pool* self) {
        if (self) {
                free(self->unused_indices);
                free(self->slots);
                free(self);
        }
}

aout_id aout_id_pool_id_create(
                aout_id_pool* self) {
        assert(self); assert(self->unused_indices); assert(self->slots);
        assert(self->unused_indices_top <= self->size);

        if (self->unused_indices_top == 0) {
                return AOUT_ID_POOL_INVALID_ID;
        }

        --self->unused_indices_top;
        size_t index = self->unused_indices[self->unused_indices_top];
        assert(index < self->size);

        aout_slot* slot = &self->slots[index];
        slot->in_use = true;
        return aout_id_make(index, slot->generation);
}

void aout_id_pool_id_destroy(
                aout_id_pool* self,
                aout_id id) {
        assert(self); assert(self->unused_indices); assert(self->slots);
        assert(id != AOUT_ID_POOL_INVALID_ID);
        assert(self->unused_indices_top < self->size);

        size_t index = aout_id_index(id);
        assert(index < self->size);

        // Protect against double free in debug mode
        assert(aout_id_pool_id_is_valid(self, id));
        for (size_t i = 0; i < self->unused_indices_top; ++i) {
                assert(self->unused_indices[i] != index);
        }

        self->unused_indices[self->unused_indices_top] = index;
        ++self->unused_indices_top;

        aout_slot* slot = &self->slots[index];
        ++slot->generation;
        slot->in_use = false;
}

bool aout_id_pool_id_is_valid(
                aout_id_pool const* self,
                aout_id id) {
        assert(self); assert(self->slots);

        if (id == AOUT_ID_POOL_INVALID_ID) {
                return false;
        }

        size_t index = aout_id_index(id);
        assert(index < self->size);

        aout_slot* slot = &self->slots[index];
        return slot->in_use && slot->generation == aout_id_generation(id);
}
