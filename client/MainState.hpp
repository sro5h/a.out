#pragma once

#include <common/State.hpp>
#include <common/StateStack.hpp>
#include <common/Host.hpp>

class MainState final : public aout::State {
public:
        explicit MainState(aout::StateStack& stateStack, aout::Host& host);
        ~MainState();

        bool onStart() override;

        bool onUpdate(aout::Time elapsed) override;
        void onRender(aout::Time elapsed) override;

private:
        void onMessageConnect(const aout::Peer& peer);
        void onMessageDisconnect(const aout::Peer& peer);
        void onMessageReceive(const aout::Peer& peer, const aout::Packet& packet);

        aout::Host& mHost;
};
