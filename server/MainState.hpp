#pragma once

#include <common/State.hpp>
#include <common/StateStack.hpp>
#include <common/Host.hpp>

class MainState final : public aout::State {
public:
        explicit MainState(aout::StateStack& stateStack, aout::Host& host);
        ~MainState();

        bool onUpdate(sf::Time elapsed) override;
        void onRender(sf::Time elapsed) override;

private:
        void onConnectMessage(const aout::Peer& peer);
        void onDisconnectMessage(const aout::Peer& peer);
        void onReceiveMessage(const aout::Peer& peer, const aout::Packet& packet);

        aout::Host& mHost;
};
