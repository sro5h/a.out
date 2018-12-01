#include "ClientApplication.hpp"

#include <common/Log.hpp>
#include <memory>

int main(int argc, char** argv) {
        auto application = std::make_unique<ClientApplication>();
        if (application) {
                application->start();

        } else {
                AOUT_LOG_ERROR("Could not allocate application");
        }

        return 0;
}
