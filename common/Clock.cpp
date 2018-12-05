#include "Clock.hpp"

namespace aout {

Clock::Clock()
        : mLastTime(InternalClock::now()) {
}

Time Clock::restart() {
        TimePoint now = InternalClock::now();
        auto elapsed = std::chrono::duration_cast<Time::Nanoseconds>(now - mLastTime);
        mLastTime = now;

        return Time(elapsed);
}

}
