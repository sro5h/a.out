#include "Log.hpp"

#include <enet/enet.h>

namespace aout { namespace {

class NetworkInitializer final {
public:
        explicit NetworkInitializer();
        ~NetworkInitializer();
};

NetworkInitializer::NetworkInitializer() {
        if (enet_initialize() != 0) {
                AOUT_LOG_ERROR("Could not initialize network module");
        }
}

NetworkInitializer::~NetworkInitializer() {
        enet_deinitialize();
}

const NetworkInitializer initializer = NetworkInitializer();

} }
