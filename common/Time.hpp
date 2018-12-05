#pragma once

#include "Config.hpp"

#include <chrono>

namespace aout {

class AOUT_API Time final {
public:
        using Nanoseconds = std::chrono::nanoseconds;

        explicit Time(int64 nanoseconds = 0);
        explicit Time(Nanoseconds nanoseconds);

        int64 asNanoseconds() const;
        int64 asMicroseconds() const;
        int64 asMilliseconds() const;
        float64 asSeconds() const;

        bool operator<(const Time& other) const;
        bool operator>(const Time& other) const;
        Time operator-() const;
        Time operator+(const Time& other) const;
        Time operator-(const Time& other) const;
        Time operator+=(const Time& other);
        Time operator-=(const Time& other);

private:
        Nanoseconds mDuration;
};

}
