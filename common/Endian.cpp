#include "Endian.hpp"

#include <limits>

#if defined(AOUT_PLATFORM_WINDOWS)
        #include <winsock2.h>
#elif defined(AOUT_PLATFORM_LINUX)
        #include <arpa/inet.h>
        #define htonll(x) htobe64((x))
        #define ntohll(x) be64toh((x)) // WARNING: OpenBSD uses betoh64
#endif

namespace aout { namespace util {

static_assert(std::numeric_limits<float32>::is_iec559, "IEC 559 floating point");
static_assert(std::numeric_limits<float64>::is_iec559, "IEC 559 floating point");

uint64 packIEC559(float64 f, uint32 bits, uint32 expBits);
float64 unpackIEC559(uint64 i, uint32 bits, uint32 expBits);

uint16 hton16(uint16 value) {
        return htons(value);
}

uint32 hton32(uint32 value) {
        return htonl(value);
}

uint64 hton64(uint64 value) {
        return htonll(value);
}

uint16 ntoh16(uint16 value) {
        return ntohs(value);
}

uint32 ntoh32(uint32 value) {
        return ntohl(value);
}

uint64 ntoh64(uint64 value) {
        return ntohll(value);
}

uint32 packIEC559_32(float32 value) {
        return static_cast<uint32>(packIEC559(value, 32, 8));
}

uint64 packIEC559_64(float64 value) {
        return packIEC559(value, 64, 11);
}

float32 unpackIEC559_32(uint32 value) {
        return static_cast<float32>(unpackIEC559(value, 32, 8));
}

float64 unpackIEC559_64(uint64 value) {
        return unpackIEC559(value, 64, 11);
}

uint64 packIEC559(float64 f, uint32 bits, uint32 expBits) {
        static_assert(sizeof(float64) == 8, "Float64 is 8 bytes");

        float64 fNorm;
        int32 shift;
        int64 sign, exponent, significand;
        uint32 significandBits = bits - expBits - 1; // -1 for sign

        if (f == 0.0) return 0;

        if (f < 0) { sign = 1; fNorm = -f; }
        else       { sign = 0; fNorm = f; }

        // Get the normalized form of f and track the exponent
        shift = 0;
        while (fNorm >= 2.0) { fNorm /= 2.0; ++shift; }
        while (fNorm  < 1.0) { fNorm *= 2.0; --shift; }
        fNorm = fNorm - 1.0;

        // Calculate the binary form (non-float) of the significand data
        significand = fNorm * ((1LL << significandBits) + 0.5f);

        // Get the biased exponent
        exponent = shift + ((1 << (expBits - 1)) -1); // shift + bias

        return (sign << (bits - 1)) | (exponent << (bits - expBits -1)) | significand;
}

float64 unpackIEC559(uint64 i, uint32 bits, uint32 expBits) {
        static_assert(sizeof(float64) == 8, "Float64 is 8 bytes");

        float64 result;
        int64 shift;
        uint32 bias;
        uint32 significandBits = bits - expBits - 1; // -1 for sign

        if (i == 0) return 0.0;

        // Pull the significand
        result = (i & ((1LL << significandBits) - 1)); // Mask
        result /= (1LL << significandBits); // Convert back to float
        result += 1.0f; // Add the one back on

        // Deal with the exponent
        bias = (1 << (expBits - 1)) -1;
        shift = ((i >> significandBits) & ((1LL << expBits) - 1)) - bias;
        while (shift > 0) { result *= 2.0; --shift; }
        while (shift < 0) { result /= 2.0; ++shift; }

        // Sign it
        result *= (i >> (bits - 1)) & 1 ? -1.0 : 1.0;
        return result;
}

} }
