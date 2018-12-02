#include "StateStack.hpp"
#include "Log.hpp"

#include <cassert>

namespace aout {

StateStack::StateStack() = default;

StateStack::~StateStack() = default;

void StateStack::onUpdate(sf::Time elapsed) {
        // Stop when State::update returns false
        for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr) {
                if (!(*itr)->onUpdate(elapsed)) {
                        break;
                }
        }

        applyPendingActions();
}

void StateStack::onRender(sf::Time elapsed) {
        for (auto& state : mStack) {
                state->onRender(elapsed);
        }
}

void StateStack::pop() {
        mPendingActions.push_back(PendingAction(Action::Pop));
}

void StateStack::clear() {
        mPendingActions.push_back(PendingAction(Action::Clear));
}

bool StateStack::isEmpty() const {
        return mStack.empty();
}

void StateStack::applyPendingActions() {
        for (PendingAction& action : mPendingActions) {
                switch (action.action) {
                case Action::Push:
                        applyPush(std::move(action.state));
                        break;

                case Action::Pop:
                        applyPop();
                        break;

                case Action::Clear:
                        mStack.clear();
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

StateStack::PendingAction::PendingAction(Action action, std::unique_ptr<State> state)
        : action(action)
        , state(std::move(state)) {
}

}
