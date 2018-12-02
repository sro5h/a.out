#include "ServerApplication.hpp"
#include "MainState.hpp"

#include <common/Config.hpp>
#include <common/Log.hpp>
#include <common/Network.hpp>

ServerApplication::ServerApplication() = default;

bool ServerApplication::onStart() {
        AOUT_LOG_DEBUG("Server v" << aout::printVersion);

        if (!aout::network::initialize()) {
                AOUT_LOG_ERROR("Could not initialize network module");
                return false;
        }

        mHost = std::make_unique<aout::Host>();
        if (mHost == nullptr) {
                AOUT_LOG_ERROR("Could not allocate host");
                return false;
        }

        if (!mHost->create("localhost", 42424, 5)) {
                AOUT_LOG_ERROR("Could not create host");
                return false;
        }

        getStateStack().push<MainState>(*mHost);

        return true;
}

void ServerApplication::onStop() {
        aout::network::deinitialize();
}

sf::Time ServerApplication::getTimeStep() {
        return sf::microseconds(31250);
}
