#include "MainState.hpp"

#include <common/Log.hpp>
#include <common/Message.hpp>

MainState::MainState(aout::StateStack& stateStack, aout::Host& host)
        : aout::State(stateStack)
        , mHost(host) {
}

MainState::~MainState() = default;

bool MainState::onUpdate(sf::Time elapsed) {
        aout::Message message;
        while (mHost.pollMessage(message)) {
                switch (message.type) {
                case aout::Message::Type::Connect:
                        onConnectMessage(message.peer);
                        break;

                case aout::Message::Type::Disconnect:
                        onDisconnectMessage(message.peer);
                        break;

                case aout::Message::Type::Receive:
                        onReceiveMessage(message.peer, message.packet);
                        break;
                }
        }

        return true;
}

void MainState::onRender(sf::Time elapsed) {
}

void MainState::onConnectMessage(const aout::Peer& peer) {
        AOUT_LOG_DEBUG("Peer connected from " << peer.address << ":" << peer.port);
}

void MainState::onDisconnectMessage(const aout::Peer& peer) {
        AOUT_LOG_DEBUG("Peer disconnected from " << peer.address << ":" << peer.port);
}

void MainState::onReceiveMessage(const aout::Peer& peer, const aout::Packet&) {
        AOUT_LOG_DEBUG("Received message from " << peer.address << ":" << peer.port);
}
