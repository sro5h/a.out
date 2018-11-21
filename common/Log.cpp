#include "Log.hpp"

#include <chrono>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace aout { namespace log {

enum class Color : uint32 {
        Black = 0,
        Red = 1,
        Green = 2,
        Yellow = 3,
        Blue = 4,
        Magenta = 5,
        Cyan = 6,
        White = 7,
        Reset = 9,
};

class Colorizer {
public:
        explicit Colorizer(Color color);
        std::ostream& operator()(std::ostream& os) const;

private:
        Color mColor;
};

std::ostream& operator<<(std::ostream& os, const Colorizer& colorizer) {
        return colorizer(os);
}

// Uses threadsafe versions of std::localtime
std::tm localtimeSafe(std::time_t t) {
#if defined(AOUT_PLATFORM_WINDOWS) && __MSC_VER >= 1400 // MSVCRT (2005+): default is threadsafe
        return *std::localtime(&t);
#elif defined(AOUT_PLATFORM_LINUX) // POSIX has localtime_r
        std::tm temp;
        return *::localtime_r(&t, &temp);
#else
        #error Unsupported platform
#endif
}

std::string millisecondsToString(std::chrono::system_clock::time_point time) {
        std::ostringstream oss;

        auto sinceEpoch = time.time_since_epoch();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(sinceEpoch) % 1000;

        oss << std::setfill('0') << std::setw(3) << ms.count();
        return oss.str();
}

std::ostream& time(std::ostream& os) {
        auto now = std::chrono::system_clock::now();
        std::tm t = localtimeSafe(std::chrono::system_clock::to_time_t(now));

        return os << std::put_time(&t, "%T") << '.' << millisecondsToString(now);
}

std::ostream& begin(std::ostream& os, Color color, const std::string& level) {
        return os << Colorizer(color) << time << " " << level << ": ";
}

std::ostream& end(std::ostream& os) {
        return os << Colorizer(Color::Reset) << std::endl;
}

std::ostream& beginDebug(std::ostream& os) {
        return begin(os, Color::White, "D");
}

std::ostream& beginWarng(std::ostream& os) {
        return begin(os, Color::Yellow, "W");
}

std::ostream& beginError(std::ostream& os) {
        return begin(os, Color::Red, "E");
}

Colorizer::Colorizer(Color color)
        : mColor(color) {
}

std::ostream& Colorizer::operator()(std::ostream& os) const {
        return os << "\033[3" << static_cast<uint32>(mColor) << "m";
}

} }
