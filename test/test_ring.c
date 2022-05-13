#include <common/ring.h>
#include <greatest/greatest.h>

TEST ring_uint8_new(void) {
        aout_ring* r = aout_ring_new(8, 1);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint8_push_back(void) {
        aout_ring* r = aout_ring_new(5, 1);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_back(r, &(uint8_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);

        aout_ring_push_back(r, &(uint8_t) { 4 });
        aout_ring_push_back(r, &(uint8_t) { 5 });
        aout_ring_push_back(r, &(uint8_t) { 12 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 12);

        aout_ring_push_back(r, &(uint8_t) { 7 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 7);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 7);


        aout_ring_push_back(r, &(uint8_t) { 42 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 7);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 42);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint8_push_front(void) {
        aout_ring* r = aout_ring_new(5, 1);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_front(r, &(uint8_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);

        aout_ring_push_front(r, &(uint8_t) { 4 });
        aout_ring_push_front(r, &(uint8_t) { 5 });
        aout_ring_push_front(r, &(uint8_t) { 12 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 2);

        aout_ring_push_front(r, &(uint8_t) { 7 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 7);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 7);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 2);


        aout_ring_push_front(r, &(uint8_t) { 42 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 7);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 4);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint8_push_back_then_pop_front(void) {
        aout_ring* r = aout_ring_new(5, 1);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_back(r, &(uint8_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);

        aout_ring_pop_front(r);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_back(r, &(uint8_t) { 2 });
        aout_ring_push_back(r, &(uint8_t) { 4 });
        aout_ring_push_back(r, &(uint8_t) { 5 });
        aout_ring_push_back(r, &(uint8_t) { 12 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 12);

        aout_ring_pop_front(r);
        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 2);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 12);

        aout_ring_push_back(r, &(uint8_t) { 73 });
        aout_ring_push_back(r, &(uint8_t) { 13 });
        aout_ring_push_back(r, &(uint8_t) { 8 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 73);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 8);

        aout_ring_push_back(r, &(uint8_t) { 42 });
        aout_ring_push_back(r, &(uint8_t) { 37 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 73);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 37);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 73);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 37);

        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 37);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 37);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint8_push_front_then_pop_front(void) {
        aout_ring* r = aout_ring_new(5, 1);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_front(r, &(uint8_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 2);

        aout_ring_pop_front(r);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_front(r, &(uint8_t) { 2 });
        aout_ring_push_front(r, &(uint8_t) { 4 });
        aout_ring_push_front(r, &(uint8_t) { 5 });
        aout_ring_push_front(r, &(uint8_t) { 12 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 12);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 5);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 2);

        aout_ring_pop_front(r);
        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 2);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 2);

        aout_ring_push_front(r, &(uint8_t) { 73 });
        aout_ring_push_front(r, &(uint8_t) { 13 });
        aout_ring_push_front(r, &(uint8_t) { 8 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 73);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 4);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 2);

        aout_ring_push_front(r, &(uint8_t) { 42 });
        aout_ring_push_front(r, &(uint8_t) { 37 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 37);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 73);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 37);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 4)), 73);

        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint8_t*) aout_ring_front(r)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_back(r)), 73);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 0)), 42);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 1)), 8);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 2)), 13);
        ASSERT_EQ(*((uint8_t*) aout_ring_at(r, 3)), 73);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint32_new(void) {
        aout_ring* r = aout_ring_new(8, sizeof(uint32_t));

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint32_push_back(void) {
        aout_ring* r = aout_ring_new(5, sizeof(uint32_t));

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_back(r, &(uint32_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);

        aout_ring_push_back(r, &(uint32_t) { 732000 });
        aout_ring_push_back(r, &(uint32_t) { 5 });
        aout_ring_push_back(r, &(uint32_t) { 3290932 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 3290932);

        aout_ring_push_back(r, &(uint32_t) { 7 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 7);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 7);


        aout_ring_push_back(r, &(uint32_t) { 42 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 5);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 7);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 42);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint32_push_front(void) {
        aout_ring* r = aout_ring_new(5, sizeof(uint32_t));

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_front(r, &(uint32_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);

        aout_ring_push_front(r, &(uint32_t) { 732000 });
        aout_ring_push_front(r, &(uint32_t) { 5 });
        aout_ring_push_front(r, &(uint32_t) { 3290932 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 5);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 2);

        aout_ring_push_front(r, &(uint32_t) { 7 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 7);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 7);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 5);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 2);


        aout_ring_push_front(r, &(uint32_t) { 42 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 732000);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 7);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 3290932);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 5);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 732000);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint32_push_back_then_pop_front(void) {
        aout_ring* r = aout_ring_new(5, sizeof(uint32_t));

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_back(r, &(uint32_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);

        aout_ring_pop_front(r);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_back(r, &(uint32_t) { 2 });
        aout_ring_push_back(r, &(uint32_t) { 4 });
        aout_ring_push_back(r, &(uint32_t) { 32902399 });
        aout_ring_push_back(r, &(uint32_t) { 12 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 12);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 4);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 32902399);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 12);

        aout_ring_pop_front(r);
        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 2);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 32902399);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 12);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 32902399);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 12);

        aout_ring_push_back(r, &(uint32_t) { 73 });
        aout_ring_push_back(r, &(uint32_t) { 13 });
        aout_ring_push_back(r, &(uint32_t) { 8 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 32902399);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 32902399);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 12);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 73);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 8);

        aout_ring_push_back(r, &(uint32_t) { 42 });
        aout_ring_push_back(r, &(uint32_t) { 89032099 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 73);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 89032099);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 73);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 89032099);

        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 89032099);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 89032099);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

TEST ring_uint32_push_front_then_pop_front(void) {
        aout_ring* r = aout_ring_new(5, sizeof(uint32_t));

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_front(r, &(uint32_t) { 2 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 1);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 2);

        aout_ring_pop_front(r);

        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_push_front(r, &(uint32_t) { 2 });
        aout_ring_push_front(r, &(uint32_t) { 4 });
        aout_ring_push_front(r, &(uint32_t) { 32902399 });
        aout_ring_push_front(r, &(uint32_t) { 12 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 12);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 12);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 32902399);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 4);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 2);

        aout_ring_pop_front(r);
        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 2);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 4);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 4);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 2);

        aout_ring_push_front(r, &(uint32_t) { 73 });
        aout_ring_push_front(r, &(uint32_t) { 13 });
        aout_ring_push_front(r, &(uint32_t) { 8 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 2);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 73);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 4);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 2);

        aout_ring_push_front(r, &(uint32_t) { 42 });
        aout_ring_push_front(r, &(uint32_t) { 89032099 });

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 5);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 89032099);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 73);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 89032099);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 4)), 73);

        aout_ring_pop_front(r);

        ASSERT(!aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 4);
        ASSERT_NEQ(aout_ring_begin(r), aout_ring_end(r));
        ASSERT_EQ(*((uint32_t*) aout_ring_front(r)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_back(r)), 73);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 0)), 42);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 1)), 8);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 2)), 13);
        ASSERT_EQ(*((uint32_t*) aout_ring_at(r, 3)), 73);

        aout_ring_clear(r);
        ASSERT(aout_ring_empty(r));
        ASSERT_EQ(aout_ring_size(r), 0);
        ASSERT_EQ(aout_ring_begin(r), aout_ring_end(r));

        aout_ring_del(&r);

        ASSERT(!r);
        PASS();
}

SUITE(test_ring) {
        RUN_TEST(ring_uint8_new);
        RUN_TEST(ring_uint8_push_back);
        RUN_TEST(ring_uint8_push_front);
        RUN_TEST(ring_uint8_push_back_then_pop_front);
        RUN_TEST(ring_uint8_push_front_then_pop_front);

        RUN_TEST(ring_uint32_new);
        RUN_TEST(ring_uint32_push_back);
        RUN_TEST(ring_uint32_push_front);
        RUN_TEST(ring_uint32_push_back_then_pop_front);
        RUN_TEST(ring_uint32_push_front_then_pop_front);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_ring);

        GREATEST_MAIN_END();
}
