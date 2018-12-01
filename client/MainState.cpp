#include "MainState.hpp"

#include <common/Log.hpp>
#include <common/Message.hpp>

MainState::MainState(aout::StateStack& stateStack, aout::Host& host)
        : aout::State(stateStack)
        , mHost(host) {
}

MainState::~MainState() = default;

bool MainState::onUpdate(sf::Time elapsed) {
        aout::Message message;
        while (mHost.pollMessage(message)) {
                AOUT_LOG_DEBUG("Message received");
        }

        return true;
}

void MainState::onRender(sf::Time elapsed) {
}
