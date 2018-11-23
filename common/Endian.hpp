#pragma once

#include "Config.hpp"

namespace aout { namespace util {

AOUT_API uint16 hton16(uint16 value);
AOUT_API uint32 hton32(uint32 value);
AOUT_API uint64 hton64(uint64 value);

AOUT_API uint16 ntoh16(uint16 value);
AOUT_API uint32 ntoh32(uint32 value);
AOUT_API uint64 ntoh64(uint64 value);

AOUT_API uint32 packIEC559_32(float32 value);
AOUT_API uint64 packIEC559_64(float64 value);

AOUT_API float32 unpackIEC559_32(uint32 value);
AOUT_API float64 unpackIEC559_64(uint64 value);

} }
