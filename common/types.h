#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdint.h>

typedef int8_t   aout_s8;
typedef uint8_t  aout_u8;
typedef int16_t  aout_s16;
typedef uint16_t aout_u16;
typedef int32_t  aout_s32;
typedef uint32_t aout_u32;
typedef int64_t  aout_s64;
typedef uint64_t aout_u64;

typedef float    aout_f32;
typedef double   aout_f64;

static_assert(sizeof(aout_f32) == 4, "aout_f32 must be 4 bytes");
static_assert(sizeof(aout_f64) == 8, "aout_f64 must be 8 bytes");

typedef ssize_t  aout_ssize;
typedef size_t   aout_usize;
typedef int8_t   aout_sbyte;
typedef uint8_t  aout_ubyte;

#endif
