#include <greatest/greatest.h>
#include <common/dummy.h>

TEST dummy_should_return_ok(void) {
        ASSERT(AOUT_IS_OK(aout_dummy(true)));
        PASS();
}

TEST dummy_should_return_err(void) {
        ASSERT(AOUT_IS_ERR(aout_dummy(false)));
        PASS();
}

SUITE(test_dummy) {
        RUN_TEST(dummy_should_return_ok);
        RUN_TEST(dummy_should_return_err);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_dummy);

        GREATEST_MAIN_END();
}
