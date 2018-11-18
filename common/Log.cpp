#include "Log.hpp"

#include <ostream>
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

std::ostream& time(std::ostream& os) {
        std::time_t t = std::time(nullptr);
        return os << std::put_time(std::localtime(&t), "%H:%M:%S");
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
