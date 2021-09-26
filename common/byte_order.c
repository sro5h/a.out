#include "byte_order.h"

#if 1 // TODO: Make portable
#       include <sys/types.h>
#       include <arpa/inet.h>
#       define htonll(x) htobe64((x))
#       define ntohll(x) be64toh((x)) // WARNING: OpenBSD uses betoh64
#elif 0 // Windows specific
#       include <winsock2.h>
#else
#       error unsupported platform
#endif

#define FLOAT32_BITS     32
#define FLOAT32_EXP_BITS 8
#define FLOAT64_BITS     64
#define FLOAT64_EXP_BITS 11

#include <float.h>

static_assert(sizeof(float32_t) == 4, "Float32 is 4 bytes");
static_assert(sizeof(float64_t) == 8, "Float64 is 8 bytes");

static_assert((FLOAT32_BITS - FLOAT32_EXP_BITS) == FLT_MANT_DIG,
                "incorrect exponent size for float32_t");
static_assert((FLOAT64_BITS - FLOAT64_EXP_BITS) == DBL_MANT_DIG,
                "incorrect exponent size for float64_t");

static uint64_t packIEC559(
                float64_t f,
                uint32_t bits,
                uint32_t exp_bits);

static float64_t unpackIEC559(
                uint64_t u,
                uint32_t bits,
                uint32_t exp_bits);

uint16_t aout_hton_u16(
                uint16_t value) {
        return htons(value);
}

uint32_t aout_hton_u32(
                uint32_t value) {
        return htonl(value);
}

uint64_t aout_hton_u64(
                uint64_t value) {
        return htonll(value);
}

uint16_t aout_ntoh_u16(
                uint16_t value) {
        return ntohs(value);
}

uint32_t aout_ntoh_u32(
                uint32_t value) {
        return ntohl(value);
}

uint64_t aout_ntoh_u64(
                uint64_t value) {
        return ntohll(value);
}

uint32_t aout_hton_f32(
                float32_t value) {
        return (uint32_t) packIEC559(value, FLOAT32_BITS, FLOAT32_EXP_BITS);
}

uint64_t aout_hton_f64(
                float64_t value) {
        return packIEC559(value, FLOAT64_BITS, FLOAT64_EXP_BITS);
}

float32_t aout_ntoh_f32(
                uint32_t value) {
        return (float32_t) unpackIEC559(value, FLOAT32_BITS, FLOAT32_EXP_BITS);
}

float64_t aout_ntoh_f64(
                uint64_t value) {
        return unpackIEC559(value, FLOAT64_BITS, FLOAT64_EXP_BITS);
}

static uint64_t packIEC559(
                float64_t f,
                uint32_t bits,
                uint32_t exp_bits) {
        float64_t f_norm;
        int32_t shift;
        int64_t sign, exponent, significand;
        uint32_t significand_bits = bits - exp_bits - 1; // -1 for sign

        if (f == 0.0) return 0;

        if (f < 0) { sign = 1; f_norm = -f; }
        else       { sign = 0; f_norm = f; }

        // Get the normalized form of f and track the exponent
        shift = 0;
        while (f_norm >= 2.0) { f_norm /= 2.0; ++shift; }
        while (f_norm  < 1.0) { f_norm *= 2.0; --shift; }
        f_norm = f_norm - 1.0;

        // Calculate the binary form (non-float) of the significand data
        significand = f_norm * ((1LL << significand_bits) + 0.5f);

        // Get the biased exponent
        exponent = shift + ((1 << (exp_bits - 1)) -1); // shift + bias

        return (sign << (bits - 1)) | (exponent << (bits - exp_bits -1)) | significand;
}

static float64_t unpackIEC559(
                uint64_t u,
                uint32_t bits,
                uint32_t exp_bits) {
        float64_t result;
        int64_t shift;
        uint32_t bias;
        uint32_t significand_bits = bits - exp_bits - 1; // -1 for sign

        if (u == 0) return 0.0;

        // Pull the significand
        result = (u & ((1LL << significand_bits) - 1)); // Mask
        result /= (1LL << significand_bits); // Convert back to float
        result += 1.0f; // Add the one back on

        // Deal with the exponent
        bias = (1 << (exp_bits - 1)) -1;
        shift = ((u >> significand_bits) & ((1LL << exp_bits) - 1)) - bias;
        while (shift > 0) { result *= 2.0; --shift; }
        while (shift < 0) { result /= 2.0; ++shift; }

        // Sign it
        result *= (u >> (bits - 1)) & 1 ? -1.0 : 1.0;
        return result;
}
