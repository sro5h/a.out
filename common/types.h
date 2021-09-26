#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include "config.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef float  float32_t;
typedef double float64_t;

static_assert(sizeof(float32_t) == 4, "float32_t must be 4 bytes");
static_assert(sizeof(float64_t) == 8, "float64_t must be 8 bytes");

#endif
