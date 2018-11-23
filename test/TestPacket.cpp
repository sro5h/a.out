#include "doctest.h"

#include <common/Packet.hpp>

using namespace aout;

TEST_CASE("Packet getSize, dataLeft and clear") {
        Packet p;

        CHECK((p << static_cast<bool>(true)));
        CHECK(p.getSize() == sizeof(bool));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<int8>(0)));
        CHECK(p.getSize() == sizeof(int8));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<uint8>(0)));
        CHECK(p.getSize() == sizeof(uint8));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<int16>(0)));
        CHECK(p.getSize() == sizeof(int16));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<uint16>(0)));
        CHECK(p.getSize() == sizeof(uint16));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<int32>(0)));
        CHECK(p.getSize() == sizeof(int32));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<uint32>(0)));
        CHECK(p.getSize() == sizeof(uint32));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<int64>(0)));
        CHECK(p.getSize() == sizeof(int64));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<uint64>(0)));
        CHECK(p.getSize() == sizeof(uint64));
        CHECK(p.dataLeft() == true);
        p.clear();
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);


        CHECK((p << static_cast<float32>(0.0f)));
        CHECK(p.getSize() == sizeof(float32));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);

        CHECK((p << static_cast<float64>(0.0)));
        CHECK(p.getSize() == sizeof(float64));
        CHECK(p.dataLeft() == true);
        p.clear();
        CHECK(p.getSize() == 0);
        CHECK(p.dataLeft() == false);
}

TEST_CASE("Packet operator<< then operator>> bool") {
        Packet p;
        bool value;

        CHECK((p << static_cast<bool>(true)));
        CHECK(p.getSize() == sizeof(bool));

        CHECK((p >> value));
        CHECK(value == true);
}

TEST_CASE("Packet operator<< then operator>> int8") {
        Packet p;
        int8 value;

        CHECK((p << static_cast<int8>(-42)));
        CHECK(p.getSize() == sizeof(int8));

        CHECK((p >> value));
        CHECK(value == -42);
}

TEST_CASE("Packet operator<< then operator>> uint8") {
        Packet p;
        uint8 value;

        CHECK((p << static_cast<uint8>(42)));
        CHECK(p.getSize() == sizeof(uint8));

        CHECK((p >> value));
        CHECK(value == 42);
}

TEST_CASE("Packet operator<< then operator>> int16") {
        Packet p;
        int16 value;

        CHECK((p << static_cast<int16>(-42)));
        CHECK(p.getSize() == sizeof(int16));

        CHECK((p >> value));
        CHECK(value == -42);
}

TEST_CASE("Packet operator<< then operator>> uint16") {
        Packet p;
        uint16 value;

        CHECK((p << static_cast<uint16>(42)));
        CHECK(p.getSize() == sizeof(uint16));

        CHECK((p >> value));
        CHECK(value == 42);
}

TEST_CASE("Packet operator<< then operator>> int32") {
        Packet p;
        int32 value;

        CHECK((p << static_cast<int32>(-42)));
        CHECK(p.getSize() == sizeof(int32));

        CHECK((p >> value));
        CHECK(value == -42);
}

TEST_CASE("Packet operator<< then operator>> uint32") {
        Packet p;
        uint32 value;

        CHECK((p << static_cast<uint32>(42)));
        CHECK(p.getSize() == sizeof(uint32));

        CHECK((p >> value));
        CHECK(value == 42);
}

TEST_CASE("Packet operator<< then operator>> int64") {
        Packet p;
        int64 value;

        CHECK((p << static_cast<int64>(-42)));
        CHECK(p.getSize() == sizeof(int64));

        CHECK((p >> value));
        CHECK(value == -42);
}

TEST_CASE("Packet operator<< then operator>> uint64") {
        Packet p;
        uint64 value;

        CHECK((p << static_cast<uint64>(42)));
        CHECK(p.getSize() == sizeof(uint64));

        CHECK((p >> value));
        CHECK(value == 42);
}

TEST_CASE("Packet operator<< then operator>> float32") {
        Packet p;
        float32 value;

        CHECK((p << static_cast<float32>(3.1459f)));
        CHECK(p.getSize() == sizeof(float32));
        CHECK((p << static_cast<float32>(-3.1459f)));
        CHECK(p.getSize() == sizeof(float32) + sizeof(float32));

        CHECK((p >> value));
        CHECK(value == 3.1459f);
        CHECK((p >> value));
        CHECK(value == -3.1459f);
}

TEST_CASE("Packet operator<< then operator>> float64") {
        Packet p;
        float64 value;

        CHECK((p << static_cast<float64>(3.1459)));
        CHECK(p.getSize() == sizeof(float64));
        CHECK((p << static_cast<float64>(-3.1459)));
        CHECK(p.getSize() == sizeof(float64) + sizeof(float64));

        CHECK((p >> value));
        CHECK(value == 3.1459);
        CHECK((p >> value));
        CHECK(value == -3.1459);
}
