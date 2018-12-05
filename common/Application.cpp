#include "Application.hpp"
#include "Log.hpp"
#include "Clock.hpp"

namespace aout {

Application::Application()
        : mIsRunning(true) {
}

Application::~Application() = default;

void Application::start() {
        Clock clock;
        Time accumulator, elapsed;

        if (!onStart()) {
                AOUT_LOG_ERROR("Could not start the application");
                stop();
        }

        while (mIsRunning) {
                elapsed = clock.restart();
                accumulator += elapsed;

                const Time timeStep = getTimeStep();

                while (accumulator > timeStep) {
                        accumulator -= timeStep;

                        mStateStack.onUpdate(timeStep);
                        if (mStateStack.isEmpty()) {
                                stop();
                        }
                }

                mStateStack.onRender(elapsed);
        }

        onStop();
}

void Application::stop() {
        mIsRunning = false;
}

StateStack& Application::getStateStack() {
        return mStateStack;
}

}
