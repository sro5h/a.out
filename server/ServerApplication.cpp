#include "ServerApplication.hpp"
#include "MainState.hpp"

#include <common/Config.hpp>
#include <common/Log.hpp>

ServerApplication::ServerApplication() = default;

bool ServerApplication::onStart() {
        AOUT_LOG_DEBUG("Server v" << aout::printVersion);

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
}

aout::Time ServerApplication::getTimeStep() {
        return aout::Time(31250000);
}
