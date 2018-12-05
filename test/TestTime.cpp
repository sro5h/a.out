#include <doctest/doctest.h>

#include <common/Time.hpp>

using namespace aout;

TEST_CASE("Default constructor") {
        Time time;

        CHECK(time.asNanoseconds() == 0);
        CHECK(time.asMicroseconds() == 0);
        CHECK(time.asMilliseconds() == 0);
        CHECK(time.asSeconds() == 0.0f);
}

TEST_CASE("Construct from positive int64") {
        Time time(12345678);

        CHECK(time.asNanoseconds() == 12345678);
        CHECK(time.asMicroseconds() == 12345);
        CHECK(time.asMilliseconds() == 12);
}

TEST_CASE("Construct from negative int64") {
        Time time(-12345678);

        CHECK(time.asNanoseconds() == -12345678);
        CHECK(time.asMicroseconds() == -12345);
        CHECK(time.asMilliseconds() == -12);
}

TEST_CASE("Operator <") {
        Time a, b;

        a = Time(123);
        b = Time(456);
        CHECK(a < b);

        a = Time(-123421536);
        b = Time(32109102390);
        CHECK(a < b);

        a = Time(2309230920);
        b = Time(1203902);
        CHECK(b < a);

        a = Time(-231432);
        b = Time(-210394320);
        CHECK(b < a);
}

TEST_CASE("Operator >") {
        Time a, b;

        a = Time(123);
        b = Time(456);
        CHECK(b > a);

        a = Time(-123421536);
        b = Time(32109102390);
        CHECK(b > a);

        a = Time(2309230920);
        b = Time(1203902);
        CHECK(a > b);

        a = Time(-231432);
        b = Time(-210394320);
        CHECK(a > b);
}

TEST_CASE("Unary operator -") {
        Time time;

        time = Time(3209012);
        CHECK((-time).asNanoseconds() == -3209012);

        time = Time(-12309800);
        CHECK((-time).asNanoseconds() == 12309800);
}

TEST_CASE("Operator +") {
        Time a, b;

        a = Time(200);
        b = Time(203);
        CHECK((a + b).asNanoseconds() == 403);

        a = Time(-3209);
        b = Time(1200);
        CHECK((a + b).asNanoseconds() == -2009);
}

TEST_CASE("Operator -") {
        Time a, b;

        a = Time(200);
        b = Time(203);
        CHECK((a - b).asNanoseconds() == -3);
        CHECK((b - a).asNanoseconds() == 3);

        a = Time(-3209);
        b = Time(1200);
        CHECK((a - b).asNanoseconds() == -4409);
        CHECK((b - a).asNanoseconds() == 4409);
}

TEST_CASE("Operator +=") {
        Time time;

        time = Time(200);
        time += Time(203);
        CHECK(time.asNanoseconds() == 403);

        time = Time(-3209);
        time += Time(1200);
        CHECK(time.asNanoseconds() == -2009);
}

TEST_CASE("Operator -=") {
        Time time;

        time = Time(3000);
        time -= Time(40);
        CHECK(time.asNanoseconds() == 2960);

        time = Time(200);
        time -= Time(203);
        CHECK(time.asNanoseconds() == -3);

        time = Time(-3209);
        time -= Time(1200);
        CHECK(time.asNanoseconds() == -4409);
}
