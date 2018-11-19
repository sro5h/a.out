#include "Host.hpp"
#include "Event.hpp"

#include <enet/enet.h>
#include <cassert>

namespace aout { namespace net {

_ENetPacket* createENetPacket(const Packet& packet);

bool initialize() {
        return enet_initialize() == 0;
}

void deinitialize() {
        enet_deinitialize();
}

Host::Host()
        : mHost(nullptr) {
}

Host::~Host() {
        destroy();
}

bool Host::create(const std::string& address, uint16 port, std::size_t connections) {
        assert(mHost == nullptr);

        _ENetAddress enetAddress;
        if (setENetAddressHost(enetAddress, address)) {
                enetAddress.port = port;
                mHost = enet_host_create(&enetAddress, connections, 2, 0, 0);
        }

        return mHost != nullptr;
}

void Host::destroy() {
        if (mHost) {
                for (std::size_t i = 0; i < mHost->peerCount; ++i) {
                        enet_peer_disconnect_now(&mHost->peers[i], 0);
                }

                enet_host_destroy(mHost);
                mHost = nullptr;
        }
}

bool Host::connect(const std::string& address, uint16 port) {
        assert(mHost != nullptr);

        _ENetPeer* peer = nullptr;
        _ENetAddress enetAddress;

        if (setENetAddressHost(enetAddress, address)) {
                enetAddress.port = port;
                peer = enet_host_connect(mHost, &enetAddress, 2, 0);
        }

        return peer != nullptr;
}

void Host::disconnect(const Peer& peer) {
        assert(mHost != nullptr);

        enet_peer_disconnect(peer.mPeer, 0);
}

void Host::disconnectAll() {
        assert(mHost != nullptr);

        for (std::size_t i = 0; i < mHost->peerCount; ++i) {
                enet_peer_disconnect(&mHost->peers[i], 0);
        }
}

bool Host::pollEvent(Event& event) {
        assert(mHost != nullptr);

        _ENetEvent enetEvent;
        if (enet_host_service(mHost, &enetEvent, 0) > 0) {
                // TODO: Store enetEvent.peer->connectId
                event.convertFrom(enetEvent);
                return true;
        }

        return false;
}

void Host::flush() {
        assert(mHost != nullptr);

        enet_host_flush(mHost);
}

bool Host::send(const Peer& peer, const Packet& packet) {
        assert(mHost != nullptr);
        assert(peer.isValid());

        _ENetPacket* enetPacket = createENetPacket(packet);

        int result = -1;
        if (enetPacket != nullptr) {
                result = enet_peer_send(peer.mPeer, 0, enetPacket);

                if (enetPacket->referenceCount == 0) {
                        enet_packet_destroy(enetPacket);
                }
        }

        return result == 0;
}

bool Host::broadcast(const Packet& packet) {
        assert(mHost != nullptr);

        _ENetPacket* enetPacket = createENetPacket(packet);
        if (enetPacket != nullptr) {
                enet_host_broadcast(mHost, 0, enetPacket);
        }

        return enetPacket != nullptr;
}

bool Host::broadcastExcept(const Peer& peer, const Packet& packet) {
        assert(mHost != nullptr);
        assert(peer.isValid());

        int result = -1;
        _ENetPacket* enetPacket = createENetPacket(packet);

        if (enetPacket != nullptr) {
                for (std::size_t i = 0; i < mHost->peerCount; ++i) {
                        if (mHost->peers[i].state != ENET_PEER_STATE_CONNECTED) {
                                continue;
                        }

                        if (&mHost->peers[i] != peer.mPeer) {
                                enet_peer_send(&mHost->peers[i], 0, enetPacket);
                        }
                }

                if (enetPacket->referenceCount == 0) {
                        enet_packet_destroy(enetPacket);
                }

                result = 0;
        }

        return result == 0;
}

bool Host::isValid() const {
        return mHost != nullptr;
}

std::size_t Host::getConnectionCount() const {
        return mHost == nullptr ? 0 : mHost->connectedPeers;
}

bool Host::setENetAddressHost(_ENetAddress& address, const std::string& host) const {
        bool result = false;
        if (host.empty()) {
                address.host = ENET_HOST_ANY;
                result = true;

        } else {
                result = enet_address_set_host(&address, host.c_str()) == 0;
        }

        return result;
}

_ENetPacket* createENetPacket(const Packet& packet) {
        uint32 enetFlags = 0;

        switch (packet.getFlag()) {
        case Packet::Flag::Unreliable:
                enetFlags |= ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT | ENET_PACKET_FLAG_UNSEQUENCED;
                break;

        case Packet::Flag::Sequenced:
                enetFlags |= ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT;
                break;

        case Packet::Flag::Reliable:
                enetFlags |= ENET_PACKET_FLAG_RELIABLE;
                break;
        }

        return enet_packet_create(packet.getData(), packet.getSize(), enetFlags);
}

} }
