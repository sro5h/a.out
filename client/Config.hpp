#pragma once

constexpr unsigned VERSION_MAJOR = 0;
constexpr unsigned VERSION_MINOR = 1;
constexpr unsigned VERSION_PATCH = 0;

std::ostream& PrintVersion(std::ostream& os) {
        return os << VERSION_MAJOR << "." << VERSION_MINOR << "."
                  << VERSION_PATCH;
}
