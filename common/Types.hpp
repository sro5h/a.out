#pragma once

#include <cstdint>

namespace aout {

using int8 = std::int8_t;
using uint8 = std::uint8_t;
using int16 = std::int16_t;
using uint16 = std::uint16_t;
using int32 = std::int32_t;
using uint32 = std::uint32_t;
using int64 = std::int64_t;
using uint64 = std::uint64_t;

static_assert(sizeof(float) == 4, "Float is 4 bytes");
static_assert(sizeof(double) == 8, "Double is 8 bytes");

using float32 = float;
using float64 = double;

}
