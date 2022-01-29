#include "tick.h"

aout_tick aout_tick_increment(
                aout_tick tick,
                uint64_t amount) {
        assert(tick.value + amount <= UINT64_MAX);

        return (aout_tick) {
                .value = tick.value + amount
        };
}

aout_tick aout_tick_decrement(
                aout_tick tick,
                uint64_t amount) {
        assert(tick.value >= amount);

        return (aout_tick) {
                .value = tick.value - amount
        };
}

bool aout_tick_filter_rate(
                aout_tick tick,
                uint64_t rate) {
        assert(rate != 0);

        return (tick.value % rate) == 0;
}

int32_t aout_tick_cmp(
                aout_tick a,
                aout_tick b) {
        if (a.value < b.value) {
                return -1;
        } else if (a.value > b.value) {
                return 1;
        } else {
                return 0;
        }
}

int64_t aout_tick_diff(
                aout_tick a,
                aout_tick b) {
        return ((int64_t) a.value) - ((int64_t) b.value);
}
