#pragma once

#include "Config.hpp"
#include "State.hpp"
#include "Factory.hpp"

#include <SFML/System/Time.hpp>
#include <vector>
#include <memory>

namespace aout {

using StateFactory = Factory<State, uint32>;

class AOUT_API StateStack final {
public:
        explicit StateStack();
        ~StateStack();

        template <typename ConcreteState, typename... Args>
        void registerState(uint32 id, Args&&... args);

        void onUpdate(sf::Time elapsed);
        void onRender(sf::Time elapsed);

        void push(uint32 id);
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
                explicit PendingAction(Action action, uint32 id = 0);

                Action action;
                uint32 id;
        };

        void applyPendingActions();
        void applyPush(uint32 id);
        void applyPop();

        std::vector<std::unique_ptr<State>> mStack;
        std::vector<PendingAction> mPendingActions;
        StateFactory mFactory;
};

template <typename ConcreteState, typename... Args>
void StateStack::registerState(uint32 id, Args&&... args) {
        mFactory.registerType<ConcreteState, StateStack&, Args...>(id, *this,
                        std::forward<Args>(args)...);
}

}
