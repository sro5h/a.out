#include "Network.hpp"

#include <enet/enet.h>

namespace aout { namespace network {

bool initialize() {
        return enet_initialize() == 0;
}

void deinitialize() {
        enet_deinitialize();
}

} }
