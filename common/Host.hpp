#pragma once

#include "Config.hpp"

#include <unordered_map>

struct _ENetAddress;
struct _ENetPeer;
struct _ENetPacket;
struct _ENetHost;

namespace aout { namespace net {

class Peer;
class Packet;
class Event;

AOUT_API bool initialize();
AOUT_API void deinitialize();

class AOUT_API Host {
public:
        explicit Host();
        virtual ~Host();

        bool create(const std::string& address, uint16 port, std::size_t connections);
        void destroy();

        bool connect(const std::string& address, uint16 port);
        void disconnect(const Peer& peer);
        void disconnectAll();

        bool pollEvent(Event& event);
        void flush();

        bool send(const Peer& peer, const Packet& packet);
        bool broadcast(const Packet& packet);
        bool broadcastExcept(const Peer& peer, const Packet& packet);

        bool isValid() const;
        std::size_t getConnectionCount() const;

private:
        void onConnectEvent(Event& event, _ENetPeer& enetPeer);
        void onDisconnectEvent(Event& event, _ENetPeer& enetPeer);
        void onReceiveEvent(Event& event, _ENetPeer& enetPeer, _ENetPacket& enetPacket);

        bool setENetAddressHost(_ENetAddress& address, const std::string& host) const;

        _ENetHost* mHost;
        std::unordered_map<_ENetPeer*, uint32> mPeerIds;
};

} }
