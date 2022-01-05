#include "ring.h"

#include <stdlib.h>
#include <string.h>

static size_t aout_ring_index(
                aout_ring const* self,
                size_t index);

static size_t aout_ring_byte_index(
                aout_ring const* self,
                size_t index);

aout_ring* aout_ring_create(
                size_t capacity,
                size_t value_size) {
        aout_ring* self = calloc(1, sizeof(*self));

        if (!self) {
                goto error;
        }

        // Reserve one dummy value at the end
        self->values = calloc(capacity + 1, value_size);

        if (!self->values) {
                goto error;
        }

        self->capacity = capacity;
        self->value_size = value_size;
        self->head = capacity;

        return self;

error:
        aout_ring_destroy(self);
        return NULL;
}

void aout_ring_destroy(
                aout_ring* self) {
        if (self) {
                free(self->values);
                free(self);
        }
}

void aout_ring_push_back(
                aout_ring* self,
                void const* value) {
        assert(self); assert(value);

        self->head = aout_ring_end(self);

        // Check whether ring was already full
        if (aout_ring_end(self) == self->tail) {
                self->tail = aout_ring_index(self, self->tail + 1);
        } else {
                ++self->size;
        }

        unsigned char* bytes = self->values;
        memcpy(
                bytes + aout_ring_byte_index(self, self->head),
                value,
                self->value_size
        );
}

void aout_ring_pop_front(
                aout_ring* self) {
        assert(self);

        if (aout_ring_empty(self)) {
                return;
        }

        self->tail = aout_ring_index(self, self->tail + 1);
        --self->size;
}

void* aout_ring_at(
                aout_ring const* self,
                size_t index) {
        assert(self);
        // Overflowing indices are ok, e.g. while iterating.
        //assert(index < aout_ring_size(self));

        unsigned char* values = self->values;
        return values + aout_ring_byte_index(self, self->tail + index);
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
        return self->tail;
}

size_t aout_ring_end(
                aout_ring const* self) {
        assert(self);
        return aout_ring_index(self, self->head + 1);
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

static size_t aout_ring_index(
                aout_ring const* self,
                size_t index) {
        return index % (self->capacity + 1);
}

static size_t aout_ring_byte_index(
                aout_ring const* self,
                size_t index) {
        return aout_ring_index(self, index) * self->value_size;
}
