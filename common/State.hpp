#pragma once

#include "Config.hpp"
#include "Time.hpp"

namespace aout {

class StateStack;

class AOUT_API State {
public:
        explicit State(StateStack& stateStack);
        virtual ~State();

        virtual bool onStart();
        virtual void onStop();

        virtual bool onUpdate(Time elapsed) = 0;
        virtual void onRender(Time elapsed) = 0;

protected:
        StateStack& getStateStack();

private:
        StateStack& mStateStack;
};

};
