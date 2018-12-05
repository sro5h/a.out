#pragma once

#include "Config.hpp"
#include "State.hpp"
#include "Time.hpp"

#include <vector>
#include <memory>

namespace aout {

class AOUT_API StateStack final {
public:
        explicit StateStack();
        ~StateStack();

        void onUpdate(Time elapsed);
        void onRender(Time elapsed);

        template <typename ConcreteState, typename... Args>
        void push(Args&&... args);
        void pop();
        void clear();

        bool isEmpty() const;

private:
        enum class Action {
                Push,
                Pop,
                Clear,
        };

        struct PendingAction final {
                explicit PendingAction(Action action, std::unique_ptr<State> state = nullptr);

                Action action;
                std::unique_ptr<State> state;
        };

        void applyPendingActions();
        void applyPush(std::unique_ptr<State> state);
        void applyPop();

        std::vector<std::unique_ptr<State>> mStack;
        std::vector<PendingAction> mPendingActions;
};

template <typename ConcreteState, typename... Args>
void StateStack::push(Args&&... args) {
        auto state = std::make_unique<ConcreteState>(*this, std::forward<Args>(args)...);
        mPendingActions.push_back(PendingAction(Action::Push, std::move(state)));
}

}
