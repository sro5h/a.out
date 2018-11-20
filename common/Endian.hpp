#pragma once

#include "Config.hpp"

namespace aout { namespace net {

AOUT_API uint16 htoni16(uint16 value);
AOUT_API uint32 htoni32(uint32 value);
AOUT_API uint64 htoni64(uint64 value);

AOUT_API uint32 htonf32(float32 value);
AOUT_API uint64 htonf64(float64 value);

AOUT_API uint16 ntohi16(uint16 value);
AOUT_API uint32 ntohi32(uint32 value);
AOUT_API uint64 ntohi64(uint64 value);

AOUT_API float32 ntohf32(uint32 value);
AOUT_API float64 ntohf64(uint64 value);

AOUT_API uint32 packIEC559f32(float32 value);
AOUT_API uint64 packIEC559f64(float64 value);

AOUT_API float32 unpackIEC559f32(uint32 value);
AOUT_API float64 unpackIEC559f64(uint64 value);

} }
