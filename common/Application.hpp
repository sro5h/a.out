#pragma once

#include "Config.hpp"
#include "StateStack.hpp"
#include "Time.hpp"

namespace aout {

class AOUT_API Application {
public:
        explicit Application();
        virtual ~Application();

        void start();
        void stop();

        virtual bool onStart() = 0;
        virtual void onStop() = 0;

        virtual Time getTimeStep() = 0;

protected:
        StateStack& getStateStack();

private:
        bool mIsRunning;
        StateStack mStateStack;
};

}
