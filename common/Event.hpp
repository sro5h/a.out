#pragma once

#include "Packet.hpp"
#include "Peer.hpp"

struct _ENetEvent;

namespace aout { namespace net {

struct AOUT_API Event final {
        enum class Type : uint16 {
                Connect,
                Disconnect,
                Receive,
        };

        Type type;
        Peer peer;
        Packet packet;

private:
        void convertFrom(const _ENetEvent& enetEvent);

        friend class Host;
};

} }
