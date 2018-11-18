#include "Config.hpp"

namespace aout {

std::ostream& printVersion(std::ostream& os) {
        return os << versionMajor << "." << versionMinor << "." << versionPatch;
}

}
