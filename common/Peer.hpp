#pragma once

#include "Config.hpp"

struct _ENetPeer;

namespace aout {

class AOUT_API Peer final {
public:
        enum class State : uint16 {
                Zombie,
                Disconnected,
                Disconnecting,
                AcknowledgingDisconnect,
                DisconnectLater,
                Connected,
                Connecting,
                AcknowledgingConnect,
                PendingConnect,
                Succeeded,
        };

        explicit Peer();
        ~Peer();

        bool isValid() const;
        State getState() const;

        bool operator==(const Peer& other) const;
        bool operator<(const Peer& other) const;

public:
        uint32 id;
        std::string address;
        uint16 port;

private:
        void convertFrom(_ENetPeer& enetPeer);

private:
        _ENetPeer* mPeer;

        friend class Host;
};

}
