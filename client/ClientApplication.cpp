#include "ClientApplication.hpp"
#include "MainState.hpp"

#include <common/Config.hpp>
#include <common/Log.hpp>
#include <common/Network.hpp>

ClientApplication::ClientApplication() = default;

bool ClientApplication::onStart() {
        AOUT_LOG_DEBUG("Client v" << aout::printVersion);

        if (!aout::network::initialize()) {
                AOUT_LOG_ERROR("Could not initialize network module");
                return false;
        }

        mHost = std::make_unique<aout::Host>();
        if (mHost == nullptr) {
                AOUT_LOG_ERROR("Could not allocate host");
                return false;
        }

        if (!mHost->create("localhost", 0, 1)) {
                AOUT_LOG_ERROR("Could not create host");
                return false;
        }

        if (!mHost->connect("localhost", 42424)) {
                AOUT_LOG_ERROR("Connection attempt failed");
                return false;
        }

        getStateStack().push<MainState>(*mHost);

        return true;
}

void ClientApplication::onStop() {
        aout::network::deinitialize();
}

sf::Time ClientApplication::getTimeStep() {
        return sf::microseconds(31250);
}
