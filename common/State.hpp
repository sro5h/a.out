#pragma once

#include "Config.hpp"

#include <SFML/System/Time.hpp>

namespace aout {

class StateStack;

class AOUT_API State {
public:
        explicit State(StateStack& stateStack);
        virtual ~State();

        virtual bool onUpdate(sf::Time elapsed) = 0;
        virtual void onRender(sf::Time elapsed) = 0;

protected:
        StateStack& getStateStack();

private:
        StateStack& mStateStack;
};

};
