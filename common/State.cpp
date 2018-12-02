#include "State.hpp"
#include "StateStack.hpp"

namespace aout {

State::State(StateStack& stateStack)
        : mStateStack(stateStack) {
}

State::~State() = default;

bool State::onStart() {
        return true;
}

void State::onStop() {
}

StateStack& State::getStateStack() {
        return mStateStack;
}

}
