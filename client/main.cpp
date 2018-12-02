#include "ClientApplication.hpp"

#include <common/Log.hpp>
#include <common/Console.hpp>
#include <memory>

int main(int argc, char** argv) {
        auto application = std::make_unique<ClientApplication>();

        aout::onSigInt([&application]() {
                AOUT_LOG_DEBUG("Received sig int, stopping application");
                application->stop();
        });

        if (application) {
                application->start();

        } else {
                AOUT_LOG_ERROR("Could not allocate application");
        }

        return 0;
}
