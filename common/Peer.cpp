#include "Peer.hpp"

#include <enet/enet.h>
#include <functional>

namespace aout {

std::string convertENetAddress(const _ENetAddress& address);

Peer::Peer()
        : mPeer(nullptr) {
}

Peer::~Peer() = default;

bool Peer::isValid() const {
        return mPeer != nullptr;
}

Peer::State Peer::getState() const {
        if (mPeer == nullptr) {
                return State::Zombie;
        }

        switch (mPeer->state) {
        case ENET_PEER_STATE_ZOMBIE:
                return State::Zombie;

        case ENET_PEER_STATE_DISCONNECTED:
                return State::Disconnected;

        case ENET_PEER_STATE_DISCONNECTING:
                return State::Disconnecting;

        case ENET_PEER_STATE_ACKNOWLEDGING_DISCONNECT:
                return State::AcknowledgingDisconnect;

        case ENET_PEER_STATE_DISCONNECT_LATER:
                return State::DisconnectLater;

        case ENET_PEER_STATE_CONNECTED:
                return State::Connected;

        case ENET_PEER_STATE_CONNECTING:
                return State::Connecting;

        case ENET_PEER_STATE_ACKNOWLEDGING_CONNECT:
                return State::AcknowledgingConnect;

        case ENET_PEER_STATE_CONNECTION_PENDING:
                return State::PendingConnect;

        case ENET_PEER_STATE_CONNECTION_SUCCEEDED:
                return State::Succeeded;

        default:
                return State::Zombie;
        }
}

bool Peer::operator==(const Peer& other) const {
        return mPeer != nullptr && other.mPeer != nullptr && mPeer == other.mPeer;
}

bool Peer::operator<(const Peer& other) const {
        return std::less<_ENetPeer*>()(mPeer, other.mPeer);
}

void Peer::convertFrom(_ENetPeer& enetPeer) {
        mPeer = &enetPeer;
        id = enetPeer.connectID;
        address = convertENetAddress(enetPeer.address);
        port = enetPeer.address.port;
}

std::string convertENetAddress(const _ENetAddress& address) {
        char buffer[64];
        std::string str;

        if (enet_address_get_host_ip(&address, buffer, sizeof(buffer)) == 0) {
                str = buffer;
        }

        return str;
}

}
