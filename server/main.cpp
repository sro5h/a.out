#include <common/Log.hpp>
#include <common/Host.hpp>
#include <common/Event.hpp>
#include <common/Peer.hpp>

#include <memory>

using namespace aout;

void run(net::Host& host);

int main(int argc, char** argv) {
        AOUT_LOG_DEBUG("Server v" << aout::printVersion);

        if (net::initialize()) {
                std::unique_ptr<net::Host> host = std::make_unique<net::Host>();

                if (host->create("localhost", 42424, 5)) {
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

void onConnectEvent(const net::Peer& peer);
void onDisconnectEvent(const net::Peer& peer);
void onReceiveEvent(const net::Peer& peer, const net::Packet& packet);

void run(net::Host& host) {
        while (host.isValid()) {
                net::Event event;
                while (host.pollEvent(event)) {
                        switch (event.type) {
                        case net::Event::Type::Connect:
                                onConnectEvent(event.peer);
                                break;

                        case net::Event::Type::Disconnect:
                                onDisconnectEvent(event.peer);
                                break;

                        case net::Event::Type::Receive:
                                onReceiveEvent(event.peer, event.packet);
                                break;
                        }
                }
        }
}

void onConnectEvent(const net::Peer& peer) {
        AOUT_LOG_DEBUG("Peer connected from " << peer.address << ":" << peer.port);
}

void onDisconnectEvent(const net::Peer& peer) {
        AOUT_LOG_DEBUG("Peer disconnected from " << peer.address << ":" << peer.port);
}

void onReceiveEvent(const net::Peer& peer, const net::Packet& packet) {
        AOUT_LOG_DEBUG("Received message from " << peer.address << ":" << peer.port);
}
