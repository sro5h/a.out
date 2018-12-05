#pragma once

#include "Config.hpp"
#include "Time.hpp"

#include <chrono>

namespace aout {

class AOUT_API Clock final {
public:
        explicit Clock();
        Time restart();

private:
        using InternalClock = std::chrono::steady_clock;
        using TimePoint = InternalClock::time_point;

        TimePoint mLastTime;
};

}
