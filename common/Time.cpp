#include "Time.hpp"

namespace aout {

Time::Time(int64 nanoseconds)
        : mDuration(nanoseconds) {
}

Time::Time(Nanoseconds nanoseconds)
        : mDuration(nanoseconds) {
}

int64 Time::asNanoseconds() const {
        return mDuration.count();
}

int64 Time::asMicroseconds() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(mDuration).count();
}

int64 Time::asMilliseconds() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(mDuration).count();
}

float64 Time::asSeconds() const {
        return asNanoseconds() / 1000000000.0;
}

bool Time::operator<(const Time& other) const {
        return mDuration < other.mDuration;
}

bool Time::operator>(const Time& other) const {
        return mDuration > other.mDuration;
}

Time Time::operator-() const {
        return Time(-mDuration);
}

Time Time::operator+(const Time& other) const {
        return Time(mDuration + other.mDuration);
}

Time Time::operator-(const Time& other) const {
        return Time(mDuration - other.mDuration);
}

Time Time::operator+=(const Time& other) {
        mDuration += other.mDuration;
        return *this;
}

Time Time::operator-=(const Time& other) {
        mDuration -= other.mDuration;
        return *this;
}

}
