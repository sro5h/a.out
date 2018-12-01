#pragma once

#include <common/Application.hpp>
#include <common/Host.hpp>
#include <memory>

class ServerApplication final : public aout::Application {
public:
        explicit ServerApplication();

        bool onStart() override;
        void onStop() override;

        sf::Time getTimeStep() override;

private:
        std::unique_ptr<aout::Host> mHost;
};
