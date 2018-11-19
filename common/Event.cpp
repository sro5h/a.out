#include "Event.hpp"

#include <enet/enet.h>
#include <cassert>

namespace aout { namespace net {

void Event::convertFrom(const _ENetEvent& enetEvent) {
        assert(enetEvent.type != ENET_EVENT_TYPE_NONE);

        peer.convertFrom(*enetEvent.peer);

        switch (enetEvent.type) {
        case ENET_EVENT_TYPE_CONNECT:
                type = Type::Connect;
                break;

        case ENET_EVENT_TYPE_DISCONNECT:
                type = Type::Disconnect;
                break;

        case ENET_EVENT_TYPE_RECEIVE:
                type = Type::Receive;
                packet.append(enetEvent.packet->data, enetEvent.packet->dataLength);
                enet_packet_destroy(enetEvent.packet);
                break;
        }
}

} }
