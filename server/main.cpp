#include "ServerApplication.hpp"

#include <common/Log.hpp>
//#include <common/Network.hpp>
//#include <common/Host.hpp>
//#include <common/Message.hpp>
//#include <common/Peer.hpp>

#include <memory>

int main(int argc, char** argv) {
        auto application = std::make_unique<ServerApplication>();
        if (application) {
                application->start();

        } else {
                AOUT_LOG_ERROR("Could not allocate application");
        }

        return 0;
}

/*using namespace aout;

void run(Host& host);

int main(int argc, char** argv) {
        AOUT_LOG_DEBUG("Server v" << aout::printVersion);

        if (network::initialize()) {
                std::unique_ptr<Host> host = std::make_unique<Host>();

                if (host->create("localhost", 42424, 5)) {
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

void onConnectMessage(const Peer& peer);
void onDisconnectMessage(const Peer& peer);
void onReceiveMessage(const Peer& peer, const Packet& packet);

void run(Host& host) {
        while (host.isValid()) {
                Message message;
                while (host.pollMessage(message)) {
                        switch (message.type) {
                        case Message::Type::Connect:
                                onConnectMessage(message.peer);
                                break;

                        case Message::Type::Disconnect:
                                onDisconnectMessage(message.peer);
                                break;

                        case Message::Type::Receive:
                                onReceiveMessage(message.peer, message.packet);
                                break;
                        }
                }
        }
}

void onConnectMessage(const Peer& peer) {
        AOUT_LOG_DEBUG("Peer connected from " << peer.address << ":" << peer.port);
}

void onDisconnectMessage(const Peer& peer) {
        AOUT_LOG_DEBUG("Peer disconnected from " << peer.address << ":" << peer.port);
}

void onReceiveMessage(const Peer& peer, const Packet& packet) {
        AOUT_LOG_DEBUG("Received message from " << peer.address << ":" << peer.port);
}*/
