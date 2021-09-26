#ifndef COMMON_BYTE_ORDER_H
#define COMMON_BYTE_ORDER_H

#include "types.h"

COMMON_API uint16_t aout_hton_u16(uint16_t value);
COMMON_API uint32_t aout_hton_u32(uint32_t value);
COMMON_API uint64_t aout_hton_u64(uint64_t value);

COMMON_API uint16_t aout_ntoh_u16(uint16_t value);
COMMON_API uint32_t aout_ntoh_u32(uint32_t value);
COMMON_API uint64_t aout_ntoh_u64(uint64_t value);

COMMON_API uint32_t aout_hton_f32(float32_t value);
COMMON_API uint64_t aout_hton_f64(float64_t value);

COMMON_API float32_t aout_ntoh_f32(uint32_t value);
COMMON_API float64_t aout_ntoh_f64(uint64_t value);

#endif
