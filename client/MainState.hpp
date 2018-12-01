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
        aout::Host& mHost;
};
