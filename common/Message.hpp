#pragma once

#include "Packet.hpp"
#include "Peer.hpp"

struct _ENetEvent;

namespace aout {

struct AOUT_API Message final {
        enum class Type : uint16 {
                Connect,
                Disconnect,
                Receive,
        };

        Type type;
        Peer peer;
        Packet packet;
};

}
