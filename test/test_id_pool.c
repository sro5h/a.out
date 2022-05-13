#include <common/id_pool.h>
#include <greatest/greatest.h>

TEST id_make(void) {
        aout_id id = AOUT_ID_POOL_INVALID_ID;

        ASSERT_EQ(id, 0);

        id = aout_id_make(0, 0);

        ASSERT_EQ(id, 0x00000001);

        id = aout_id_make(AOUT_ID_POOL_MAX_INDEX, 0);

        ASSERT_EQ(id, 0x00ffffff);

        id = aout_id_make(0, 0xff);

        ASSERT_EQ(id, 0xff000001);

        id = aout_id_make(0, 0x3f);

        ASSERT_EQ(id, 0x3f000001);

        id = aout_id_make(0x45e271, 0x32);

        ASSERT_EQ(id, 0x3245e272);

        id = aout_id_make(AOUT_ID_POOL_MAX_INDEX, 0xff);

        ASSERT_EQ(id, 0xffffffff);

        PASS();
}

TEST id_make_then_id_index(void) {
        aout_id id = AOUT_ID_POOL_INVALID_ID;

        id = aout_id_make(0, 0);

        ASSERT_EQ(aout_id_index(id), 0);

        id = aout_id_make(0, 0xff);

        ASSERT_EQ(aout_id_index(id), 0);

        id = aout_id_make(0x54e272, 0x32);

        ASSERT_EQ(aout_id_index(id), 0x54e272);

        id = aout_id_make(AOUT_ID_POOL_MAX_INDEX, 0xff);

        ASSERT_EQ(aout_id_index(id), AOUT_ID_POOL_MAX_INDEX);

        PASS();
}

TEST id_make_then_id_generation(void) {
        aout_id id = AOUT_ID_POOL_INVALID_ID;

        id = aout_id_make(0, 0);

        ASSERT_EQ(aout_id_generation(id), 0);

        id = aout_id_make(0x20a3f8, 0xff);

        ASSERT_EQ(aout_id_generation(id), 0xff);

        id = aout_id_make(0x54e272, 0x32);

        ASSERT_EQ(aout_id_generation(id), 0x32);

        id = aout_id_make(AOUT_ID_POOL_MAX_INDEX, 0xff);

        ASSERT_EQ(aout_id_generation(id), 0xff);

        PASS();
}

TEST id_pool_new(void) {
        aout_id_pool* pool = aout_id_pool_new(0x400);

        ASSERT(pool);
        ASSERT(!aout_id_pool_id_is_valid(pool, AOUT_ID_POOL_INVALID_ID));
        ASSERT(!aout_id_pool_id_is_valid(pool, aout_id_make(0x0003ff, 0x00)));
        ASSERT(!aout_id_pool_id_is_valid(pool, aout_id_make(0x0000f8, 0xf3)));

        aout_id_pool_del(&pool);

        PASS();
}

TEST id_pool_id_new_then_id_pool_id_is_valid(void) {
        aout_id_pool* pool = aout_id_pool_new(0xff);
        aout_id id = AOUT_ID_POOL_INVALID_ID;

        ASSERT(pool);

        aout_id id0 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id0, 0x00000001);
        ASSERT(aout_id_pool_id_is_valid(pool, id0));
        ASSERT(!aout_id_pool_id_is_valid(pool, AOUT_ID_POOL_INVALID_ID));
        printf("id0: %d\n", id0);

        aout_id id1 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id1, 0x00000002);
        ASSERT(aout_id_pool_id_is_valid(pool, id0));
        ASSERT(aout_id_pool_id_is_valid(pool, id1));
        ASSERT(!aout_id_pool_id_is_valid(pool, AOUT_ID_POOL_INVALID_ID));

        for (size_t i = 2; i < 0xff; ++i) {
                id = aout_id_pool_id_new(pool);

                ASSERT_EQ(id, i + 1);
                ASSERT(aout_id_pool_id_is_valid(pool, id));
        }

        id = aout_id_pool_id_new(pool);

        ASSERT_EQ(id, AOUT_ID_POOL_INVALID_ID);
        ASSERT(aout_id_pool_id_is_valid(pool, id0));
        ASSERT(aout_id_pool_id_is_valid(pool, id1));

        aout_id_pool_del(&pool);

        PASS();
}

TEST id_pool_id_new_then_id_pool_id_del(void) {
        aout_id_pool* pool = aout_id_pool_new(0xff);

        ASSERT(pool);

        aout_id id01 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id01, 0x00000001);
        ASSERT(aout_id_pool_id_is_valid(pool, id01));

        aout_id_pool_id_del(pool, id01);

        ASSERT(!aout_id_pool_id_is_valid(pool, id01));

        aout_id id11 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id11, 0x01000001);
        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));

        aout_id id02 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id02, 0x00000002);
        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(aout_id_pool_id_is_valid(pool, id02));

        aout_id id03 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id03, 0x00000003);
        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(aout_id_pool_id_is_valid(pool, id02));
        ASSERT(aout_id_pool_id_is_valid(pool, id03));

        aout_id_pool_id_del(pool, id03);

        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(aout_id_pool_id_is_valid(pool, id02));
        ASSERT(!aout_id_pool_id_is_valid(pool, id03));

        aout_id id13 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id13, 0x01000003);
        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(aout_id_pool_id_is_valid(pool, id02));
        ASSERT(!aout_id_pool_id_is_valid(pool, id03));
        ASSERT(aout_id_pool_id_is_valid(pool, id13));

        aout_id_pool_id_del(pool, id13);

        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(aout_id_pool_id_is_valid(pool, id02));
        ASSERT(!aout_id_pool_id_is_valid(pool, id03));
        ASSERT(!aout_id_pool_id_is_valid(pool, id13));

        aout_id id23 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id23, 0x02000003);
        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(aout_id_pool_id_is_valid(pool, id02));
        ASSERT(!aout_id_pool_id_is_valid(pool, id03));
        ASSERT(!aout_id_pool_id_is_valid(pool, id13));
        ASSERT(aout_id_pool_id_is_valid(pool, id23));

        aout_id_pool_id_del(pool, id02);

        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(!aout_id_pool_id_is_valid(pool, id02));
        ASSERT(!aout_id_pool_id_is_valid(pool, id03));
        ASSERT(!aout_id_pool_id_is_valid(pool, id13));
        ASSERT(aout_id_pool_id_is_valid(pool, id23));

        aout_id id12 = aout_id_pool_id_new(pool);

        ASSERT_EQ(id12, 0x01000002);
        ASSERT(!aout_id_pool_id_is_valid(pool, id01));
        ASSERT(aout_id_pool_id_is_valid(pool, id11));
        ASSERT(!aout_id_pool_id_is_valid(pool, id02));
        ASSERT(!aout_id_pool_id_is_valid(pool, id03));
        ASSERT(!aout_id_pool_id_is_valid(pool, id13));
        ASSERT(aout_id_pool_id_is_valid(pool, id23));
        ASSERT(aout_id_pool_id_is_valid(pool, id12));

        aout_id_pool_del(&pool);

        PASS();
}

SUITE(test_id_pool) {
        RUN_TEST(id_make);
        RUN_TEST(id_make_then_id_index);
        RUN_TEST(id_make_then_id_generation);

        RUN_TEST(id_pool_new);
        RUN_TEST(id_pool_id_new_then_id_pool_id_is_valid);
        RUN_TEST(id_pool_id_new_then_id_pool_id_del);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_id_pool);

        GREATEST_MAIN_END();
}
