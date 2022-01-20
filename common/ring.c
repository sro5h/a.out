#include "ring.h"

#include <stdlib.h>
#include <string.h>

static size_t aout_ring_byte_index(
                aout_ring const* self,
                size_t index);

aout_res aout_ring_init(
                aout_ring* self,
                size_t capacity,
                size_t value_size) {
        assert(self);

        // Reserve one dummy value at the end
        self->values = calloc(capacity + 1, value_size);

        if (!self->values) {
                return AOUT_ERR;
        }

        self->capacity = capacity;
        self->value_size = value_size;

        return AOUT_OK;
}

void aout_ring_fini(
                aout_ring* self) {
        assert(self);
        free(self->values);
}

aout_ring* aout_ring_create(
                size_t capacity,
                size_t value_size) {
        aout_ring* self = calloc(1, sizeof(*self));

        if (!self) {
                goto error;
        }

        if (AOUT_IS_ERR(aout_ring_init(self, capacity, value_size))) {
                goto error;
        }

        return self;

error:
        aout_ring_destroy(self);
        return NULL;
}

void aout_ring_destroy(
                aout_ring* self) {
        if (self) {
                aout_ring_fini(self);
                free(self);
        }
}

void aout_ring_clear(
                aout_ring* self) {
        assert(self);

        self->root = 0;
        self->size = 0;
}

void aout_ring_push_back(
                aout_ring* self,
                void const* value) {
        assert(self); assert(value);

        // Check whether ring was already full
        if (self->size == self->capacity) {
                self->root = aout_ring_index(self, 1);
        } else {
                ++self->size;
        }

        unsigned char* bytes = self->values;
        memcpy(
                bytes + aout_ring_byte_index(self, self->size - 1),
                value,
                self->value_size
        );
}

void aout_ring_push(
                aout_ring* self,
                void const* value) {
        aout_ring_push_back(self, value);
}

void aout_ring_pop_front(
                aout_ring* self) {
        assert(self);

        if (aout_ring_empty(self)) {
                return;
        }

        self->root = aout_ring_index(self, 1);
        --self->size;
}

void aout_ring_pop(
                aout_ring* self) {
        aout_ring_pop_front(self);
}

void* aout_ring_at(
                aout_ring const* self,
                size_t index) {
        assert(self);
        // Overflowing indices are ok, e.g. while iterating.
        //assert(index < aout_ring_size(self));

        unsigned char* values = self->values;
        return values + aout_ring_byte_index(self, index);
}

void* aout_ring_front(
                aout_ring const* self) {
        assert(self);
        assert(!aout_ring_empty(self));
        return aout_ring_at(self, 0);
}

void* aout_ring_back(
                aout_ring const* self) {
        assert(self);
        assert(!aout_ring_empty(self));
        return aout_ring_at(self, aout_ring_size(self) - 1);
}

size_t aout_ring_begin(
                aout_ring const* self) {
        assert(self);
        return 0;
}

size_t aout_ring_end(
                aout_ring const* self) {
        assert(self);
        return aout_ring_size(self);
}

size_t aout_ring_size(
                aout_ring const* self) {
        assert(self);
        return self->size;
}

bool aout_ring_empty(
                aout_ring const* self) {
        assert(self);
        return aout_ring_size(self) == 0;
}

size_t aout_ring_index(
                aout_ring const* self,
                size_t index) {
        return (self->root + index) % (self->capacity + 1);
}

size_t aout_ring_byte_index(
                aout_ring const* self,
                size_t index) {
        return aout_ring_index(self, index) * self->value_size;
}
