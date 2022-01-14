#include "tick.h"

void aout_tick_increment(
                aout_tick* self) {
        assert(self);
        assert(self->value < UINT64_MAX);

        ++self->value;
}

bool aout_tick_filter_rate(
                aout_tick const* self,
                uint32_t rate) {
        assert(self);
        assert(rate != 0);

        return (self->value % rate) == 0;
}

COMMON_API int32_t aout_tick_cmp(
                aout_tick a,
                aout_tick b) {
        if (a.value < b. value) {
                return -1;
        } else if (a.value > b.value) {
                return 1;
        } else {
                return 0;
        }
}
