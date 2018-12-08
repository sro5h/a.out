#pragma once

#include <common/Application.hpp>
#include <common/Host.hpp>
#include <memory>

class ServerApplication final : public aout::Application {
public:
        explicit ServerApplication();

        bool onStart() override;
        void onStop() override;

        aout::Time getTimeStep() override;

private:
        aout::Host mHost;
};
