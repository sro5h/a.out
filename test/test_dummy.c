#include <greatest/greatest.h>
#include <common/dummy.h>

TEST dummy_should_return_ok(void) {
        ASSERT(aout_dummy() == AOUT_DUMMY_OK);
        PASS();
}

SUITE(test_dummy) {
        RUN_TEST(dummy_should_return_ok);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_dummy);

        GREATEST_MAIN_END();
}
