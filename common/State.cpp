#include "State.hpp"
#include "StateStack.hpp"

namespace aout {

State::State(StateStack& stateStack)
        : mStateStack(stateStack) {
}

State::~State() = default;

StateStack& State::getStateStack() {
        return mStateStack;
}

}
