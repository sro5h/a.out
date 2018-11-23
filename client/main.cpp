#include <common/Log.hpp>
#include <common/Network.hpp>
#include <common/Host.hpp>
#include <common/Message.hpp>

#include <memory>

using namespace aout;

void run(Host& host);

int main(int argc, char** argv) {
        AOUT_LOG_DEBUG("Client v" << aout::printVersion);

        if (network::initialize()) {
                std::unique_ptr<Host> host = std::make_unique<Host>();

                if (host->create("localhost", 0, 1)) {
                        run(*host);

                } else {
                        AOUT_LOG_ERROR("Could not create host");
                }

        } else {
                AOUT_LOG_ERROR("Could not initialize network module");
        }

        network::deinitialize();
        return 0;
}

void run(Host& host) {
        if (host.connect("localhost", 42424)) {
                while (true) {
                        Message message;
                        while (host.pollMessage(message)) {
                                (void)0;
                        }
                }

        }
}
