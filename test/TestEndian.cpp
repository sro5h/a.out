#include "doctest.h"

#include <common/Endian.hpp>

using namespace aout;

TEST_CASE("htoni16 then ntohi16") {
        uint16 u;
        u = net::ntohi16(net::htoni16(42424));

        CHECK(u == 42424);

        int16 i;
        i = net::ntohi16(net::htoni16(-424));

        CHECK(i == -424);
}

TEST_CASE("htoni32 then ntohi32") {
        uint32 u;
        u = net::ntohi32(net::htoni32(42424));

        CHECK(u == 42424);

        int32 i;
        i = net::ntohi32(net::htoni32(-424));

        CHECK(i == -424);
}

TEST_CASE("htoni64 then ntohi64") {
        uint64 u;
        u = net::ntohi64(net::htoni64(42424));

        CHECK(u == 42424);

        int64 i;
        i = net::ntohi64(net::htoni64(-424));

        CHECK(i == -424);
}

TEST_CASE("htonf32 then ntohf32") {
        float32 f;
        f = net::ntohf32(net::htonf32(3.1459f));

        CHECK(f == 3.1459f);

        f = net::ntohf32(net::htonf32(-3.1459f));

        CHECK(f == -3.1459f);
}

TEST_CASE("htonf64 then ntohf64") {
        float64 f;
        f = net::ntohf64(net::htonf64(3.1459));

        CHECK(f == 3.1459);

        f = net::ntohf64(net::htonf64(-3.1459));

        CHECK(f == -3.1459);
}

TEST_CASE("packIEC559f32 then unpackIEC559f32") {
        float32 f;
        f = net::unpackIEC559f32(net::packIEC559f32(3.1459f));

        CHECK(f == 3.1459f);

        f = net::unpackIEC559f32(net::packIEC559f32(-3.1459f));

        CHECK(f == -3.1459f);
}

TEST_CASE("packIEC559f64 then unpackIEC559f64") {
        float64 f;
        f = net::unpackIEC559f64(net::packIEC559f64(3.1459));

        CHECK(f == 3.1459);

        f = net::unpackIEC559f64(net::packIEC559f64(-3.1459));

        CHECK(f == -3.1459);
}
