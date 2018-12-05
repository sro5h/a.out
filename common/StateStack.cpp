#include "StateStack.hpp"
#include "Log.hpp"

#include <cassert>

namespace aout {

StateStack::StateStack() = default;

StateStack::~StateStack() {
        applyClear();
}

void StateStack::onUpdate(Time elapsed) {
        // Stop when State::update returns false
        for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
                if (!(*itr)->onUpdate(elapsed)) {
                        break;
                }
        }

        applyPendingActions();
}

void StateStack::onRender(Time elapsed) {
        for (auto& state : mStack) {
                state->onRender(elapsed);
        }
}

void StateStack::pop() {
        mPendingActions.push_back(Action(Action::Type::Pop));
}

void StateStack::clear() {
        mPendingActions.push_back(Action(Action::Type::Clear));
}

bool StateStack::isEmpty() const {
        return mStack.empty();
}

void StateStack::applyPendingActions() {
        for (Action& action : mPendingActions) {
                switch (action.type) {
                case Action::Type::Push:
                        applyPush(std::move(action.state));
                        break;

                case Action::Type::Pop:
                        applyPop();
                        break;

                case Action::Type::Clear:
                        applyClear();
                        break;
                }
        }

        mPendingActions.clear();
}

void StateStack::applyPush(std::unique_ptr<State> state) {
        if (state != nullptr) {
                if (state->onStart()) {
                        mStack.push_back(std::move(state));

                } else {
                        AOUT_LOG_ERROR("Can't push state, onStart returned false");
                        assert(false);
                }

        } else {
                AOUT_LOG_ERROR("Can't push state, is nullptr");
                assert(false);
        }
}

void StateStack::applyPop() {
        if (!mStack.empty()) {
                mStack.back()->onStop();
                mStack.pop_back();

        } else {
                AOUT_LOG_ERROR("Couldn't pop state, stack is empty");
                assert(false);
        }
}

void StateStack::applyClear() {
        for (auto& state: mStack) {
                state->onStop();
        }

        mStack.clear();
}

StateStack::Action::Action(Type type, std::unique_ptr<State> state)
        : type(type)
        , state(std::move(state)) {
}

}
