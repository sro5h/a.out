#ifndef COMMON_TICK_H
#define COMMON_TICK_H

#include "types.h"

typedef struct aout_tick {
        // TODO: Maybe reduce to uint16_t and handle wrapping correctly.
        // On the other hand 64 bit should be so much that even an increase
        // to 256 ticks per second shouldn't be a problem.
        uint64_t value;
} aout_tick;

COMMON_API void aout_tick_increment(
                aout_tick* self);

COMMON_API bool aout_tick_filter_rate(
                aout_tick const* self,
                uint32_t rate);

COMMON_API int32_t aout_tick_cmp(
                aout_tick a,
                aout_tick b);

#endif
