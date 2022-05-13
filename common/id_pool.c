#include "id_pool.h"
#include "memory.h"

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

static void aout_id_pool_ctor(
                aout_id_pool* self,
                size_t size) {
        assert(self); assert(size > 0); assert(size <= AOUT_ID_POOL_MAX_SIZE);

        // Slots must be zeroed.
        self->slots = aout_acquire(size * sizeof(*self->slots));
        self->unused_indices = aout_acquire(size * sizeof(*self->unused_indices));

        self->size = size;
        self->unused_indices_top = 0;

        // Populate unused indices with [size - 1 .. 0].
        for (size_t i = 1; i <= self->size; ++i) {
                self->unused_indices[self->unused_indices_top] = self->size - i;
                ++self->unused_indices_top;
        }
}

static void aout_id_pool_dtor(
                aout_id_pool* self) {
        assert(self);

        aout_release(self->slots);
        aout_release(self->unused_indices);
        *self = (aout_id_pool) { 0 };
}

aout_id_pool* aout_id_pool_new(
                size_t size) {
        aout_id_pool* self = aout_acquire(sizeof(*self));
        aout_id_pool_ctor(self, size);

        return self;
}

void aout_id_pool_del(
                aout_id_pool** out_self) {
        assert(out_self);

        if (*out_self) {
                aout_id_pool_dtor(*out_self);
                aout_release(*out_self);
                *out_self = NULL;
        }
}

aout_id aout_id_pool_id_new(
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

void aout_id_pool_id_del(
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
