#include "ClientApplication.hpp"
#include "MainState.hpp"

#include <common/Config.hpp>
#include <common/Log.hpp>

ClientApplication::ClientApplication() = default;

bool ClientApplication::onStart() {
        AOUT_LOG_DEBUG("Client v" << aout::printVersion);

        if (!mHost.create("localhost", 0, 1)) {
                AOUT_LOG_ERROR("Could not create host");
                return false;
        }

        getStateStack().push<MainState>(mHost);

        return true;
}

void ClientApplication::onStop() {
}

aout::Time ClientApplication::getTimeStep() {
        return aout::Time(31250000);
}
