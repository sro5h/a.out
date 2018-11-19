#include <common/Log.hpp>
#include <common/Host.hpp>
#include <common/Event.hpp>

#include <memory>

using namespace aout;

void run(net::Host& host);

int main(int argc, char** argv) {
        AOUT_LOG_DEBUG("Client v" << aout::printVersion);

        if (net::initialize()) {
                std::unique_ptr<net::Host> host = std::make_unique<net::Host>();

                if (host->create("localhost", 0, 1)) {
                        run(*host);

                } else {
                        AOUT_LOG_ERROR("Could not create host");
                }

        } else {
                AOUT_LOG_ERROR("Could not initialize net module");
        }

        net::deinitialize();
        return 0;
}

void run(net::Host& host) {
        if (host.connect("localhost", 42424)) {
                while (true) {
                        net::Event event;
                        while (host.pollEvent(event)) {
                                (void)0;
                        }
                }

        }
}
