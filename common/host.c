#include "host.h"
#include "log.h"

#include <enet/enet.h>

static aout_res aout_host_create_packet(
                aout_msg_desc* msg_desc,
                ENetPacket** out_packet);

aout_res aout_host_send_msg(
                ENetHost* self,
                uint16_t peer_id,
                aout_msg_desc* msg_desc) {
        assert(self); assert(msg_desc);

        ENetPacket* packet;
        aout_res res = aout_host_create_packet(msg_desc, &packet);

        if (AOUT_IS_ERR(res)) {
                goto error;
        }

        // Ownership of packet is transferred, if enet_peer_send succeeds!
        if (enet_peer_send(&self->peers[peer_id], 0, packet) < 0) {
                aout_loge("could not send msg(%u)", msg_desc->type);
                goto error;
        }

        return AOUT_OK;

error:
        enet_packet_destroy(packet);
        return AOUT_ERR;
}

aout_res aout_host_send_msg_to_all(
                ENetHost* self,
                aout_msg_desc* msg_desc) {
        assert(self); assert(msg_desc);

        ENetPacket* packet;
        aout_res res = aout_host_create_packet(msg_desc, &packet);

        if (AOUT_IS_ERR(res)) {
                goto error;
        }

        // Ownership of packet is transferred if enet_peer_send succeeds!
        for (size_t i = 0; i < self->peerCount; ++ i) {
                if (self->peers[i].state != ENET_PEER_STATE_CONNECTED) {
                        continue;
                }

                if (enet_peer_send(&self->peers[i], 0, packet) < 0) {
                        aout_loge("could not send msg(%u)", msg_desc->type);
                }
        }

        if (packet->referenceCount == 0) {
                enet_packet_destroy(packet);
        }

        return AOUT_OK;

error:
        enet_packet_destroy(packet);
        return AOUT_ERR;
}

aout_res aout_host_send_msg_to_all_except(
                ENetHost* self,
                uint16_t peer_id,
                aout_msg_desc* msg_desc) {
        assert(self); assert(msg_desc);

        ENetPacket* packet;
        aout_res res = aout_host_create_packet(msg_desc, &packet);

        if (AOUT_IS_ERR(res)) {
                goto error;
        }

        // Ownership of packet is transferred if enet_peer_send succeeds!
        for (size_t i = 0; i < self->peerCount; ++i) {
                ENetPeer* peer = &self->peers[i];
                if (peer->incomingPeerID == peer_id
                                || peer->state != ENET_PEER_STATE_CONNECTED) {
                        continue;
                }

                if (enet_peer_send(peer, 0, packet) < 0) {
                        aout_loge("could not send msg(%u)", msg_desc->type);
                }
        }

        if (packet->referenceCount == 0) {
                enet_packet_destroy(packet);
        }

        return AOUT_OK;

error:
        enet_packet_destroy(packet);
        return AOUT_ERR;
}

aout_res aout_host_create_packet(
                aout_msg_desc* msg_desc,
                ENetPacket** out_packet) {
        assert(msg_desc); assert(out_packet);
        assert(msg_desc->write);
        assert(msg_desc->value);
        assert(msg_desc->size > 0);

        *out_packet = enet_packet_create(
                NULL,
                sizeof(aout_msg_type) + msg_desc->size,
                msg_desc->flags
        );

        if (!*out_packet) {
                aout_loge("could not create msg(%u) buffer", msg_desc->type);
                goto error;
        }

        aout_stream stream = {
                .data = (*out_packet)->data,
                .data_size = (*out_packet)->dataLength,
        };

        static_assert(sizeof(aout_msg_type) == sizeof(uint16_t));
        aout_res res = aout_stream_write_u16(&stream, msg_desc->type);

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write msg(%u) header", msg_desc->type);
                goto error;
        }

        res = msg_desc->write(&stream, msg_desc->value);

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not write msg(%u)", msg_desc->type);
                goto error;
        }

        // Resize the packet to the number of written bytes
        enet_packet_resize(*out_packet, aout_stream_get_count(&stream));
        return AOUT_OK;

error:
        enet_packet_destroy(*out_packet);
        *out_packet = NULL;

        return AOUT_ERR;
}
