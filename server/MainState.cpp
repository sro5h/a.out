#include "MainState.hpp"

#include <common/Log.hpp>
#include <common/Message.hpp>

MainState::MainState(aout::StateStack& stateStack, aout::Host& host)
        : aout::State(stateStack)
        , mHost(host) {
}

MainState::~MainState() = default;

bool MainState::onUpdate(aout::Time elapsed) {
        aout::Message message;
        while (mHost.pollMessage(message)) {
                switch (message.type) {
                case aout::Message::Type::Connect:
                        onMessageConnect(message.peer);
                        break;

                case aout::Message::Type::Disconnect:
                        onMessageDisconnect(message.peer);
                        break;

                case aout::Message::Type::Receive:
                        onMessageReceive(message.peer, message.packet);
                        break;
                }
        }

        return true;
}

void MainState::onRender(aout::Time elapsed) {
}

void MainState::onMessageConnect(const aout::Peer& peer) {
        AOUT_LOG_DEBUG("Peer connected from " << peer.address << ":" << peer.port);
}

void MainState::onMessageDisconnect(const aout::Peer& peer) {
        AOUT_LOG_DEBUG("Peer disconnected from " << peer.address << ":" << peer.port);
}

void MainState::onMessageReceive(const aout::Peer& peer, const aout::Packet&) {
        AOUT_LOG_DEBUG("Received message from " << peer.address << ":" << peer.port);
}
