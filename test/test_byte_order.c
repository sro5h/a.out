#include <common/byte_order.h>
#include <greatest/greatest.h>

TEST hton_u16_then_ntoh_u16(void) {
        uint16_t u;
        u = aout_ntoh_u16(aout_hton_u16(42424));

        ASSERT_EQ(u, 42424);

        int16_t i;
        i = aout_ntoh_u16(aout_hton_u16(-424));

        ASSERT_EQ(i, -424);

        PASS();
}

TEST hton_u32_then_ntoh_u32(void) {
        uint32_t u;
        u = aout_ntoh_u32(aout_hton_u32(42424));

        ASSERT_EQ(u, 42424);

        int32_t i;
        i = aout_ntoh_u32(aout_hton_u32(-424));

        ASSERT_EQ(i, -424);

        PASS();
}

TEST hton_u64_then_ntoh_u64(void) {
        uint64_t u;
        u = aout_ntoh_u64(aout_hton_u64(42424));

        ASSERT_EQ(u, 42424);

        int64_t i;
        i = aout_ntoh_u64(aout_hton_u64(-424));

        ASSERT_EQ(i, -424);

        PASS();
}

TEST hton_f32_then_ntoh_f32(void) {
        float32_t f;
        f = aout_ntoh_f32(aout_hton_f32(3.1459f));

        ASSERT_EQ(f, 3.1459f);

        f = aout_ntoh_f32(aout_hton_f32(-3.1459f));

        ASSERT_EQ(f, -3.1459f);

        PASS();
}

TEST hton_f64_then_ntoh_f64(void) {
        float64_t f;
        f = aout_ntoh_f64(aout_hton_f64(3.1459));

        ASSERT_EQ(f, 3.1459);

        f = aout_ntoh_f64(aout_hton_f64(-3.1459));

        ASSERT_EQ(f, -3.1459);

        PASS();
}

SUITE(test_byte_order) {
        RUN_TEST(hton_u16_then_ntoh_u16);
        RUN_TEST(hton_u32_then_ntoh_u32);
        RUN_TEST(hton_u64_then_ntoh_u64);
        RUN_TEST(hton_f32_then_ntoh_f32);
        RUN_TEST(hton_f64_then_ntoh_f64);
}

GREATEST_MAIN_DEFS();

int main(int argc, char* argv[]) {
        GREATEST_MAIN_BEGIN();

        RUN_SUITE(test_byte_order);

        GREATEST_MAIN_END();
}
