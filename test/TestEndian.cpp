#include "doctest.h"

#include <common/Endian.hpp>

using namespace aout;

TEST_CASE("hton16 then ntoh16") {
        uint16 u;
        u = net::ntoh16(net::hton16(42424));

        CHECK(u == 42424);

        int16 i;
        i = net::ntoh16(net::hton16(-424));

        CHECK(i == -424);
}

TEST_CASE("hton32 then ntoh32") {
        uint32 u;
        u = net::ntoh32(net::hton32(42424));

        CHECK(u == 42424);

        int32 i;
        i = net::ntoh32(net::hton32(-424));

        CHECK(i == -424);
}

TEST_CASE("hton64 then ntoh64") {
        uint64 u;
        u = net::ntoh64(net::hton64(42424));

        CHECK(u == 42424);

        int64 i;
        i = net::ntoh64(net::hton64(-424));

        CHECK(i == -424);
}

TEST_CASE("packIEC559_32 then unpackIEC559_32") {
        float32 f;
        f = net::unpackIEC559_32(net::packIEC559_32(3.1459f));

        CHECK(f == 3.1459f);

        f = net::unpackIEC559_32(net::packIEC559_32(-3.1459f));

        CHECK(f == -3.1459f);
}

TEST_CASE("packIEC559_64 then unpackIEC559_64") {
        float64 f;
        f = net::unpackIEC559_64(net::packIEC559_64(3.1459));

        CHECK(f == 3.1459);

        f = net::unpackIEC559_64(net::packIEC559_64(-3.1459));

        CHECK(f == -3.1459);
}
