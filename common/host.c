#include "host.h"
#include "log.h"

#include <enet/enet.h>

static aout_res aout_host_create_packet(
                size_t size,
                uint32_t flags,
                ENetPacket** out_packet,
                aout_stream* out_stream);

aout_res aout_host_send_msg(
                ENetHost* self,
                uint16_t peer_id,
                aout_msg_desc* msg_desc) {
        assert(self); assert(msg_desc);
        assert(msg_desc->write);
        assert(msg_desc->value);
        assert(msg_desc->size > 0);

        ENetPacket* packet;
        aout_stream stream;
        aout_res res = aout_host_create_packet(
                msg_desc->size,
                msg_desc->flags,
                &packet,
                &stream
        );

        if (AOUT_IS_ERR(res)) {
                aout_loge("could not create msg(%u) buffer", msg_desc->type);
                goto error;
        }

        static_assert(sizeof(aout_msg_type) == 2);
        res = aout_stream_write_u16(&stream, msg_desc->type);

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
        enet_packet_resize(packet, aout_stream_get_count(&stream));

        // Ownership of packet is transferred, if enet_peer_send succeeds!
        if (enet_peer_send(&self->peers[peer_id], 0, packet) < 0) {
                aout_loge("could not send msg(%u)", msg_desc->type);
                goto error;
        }

        return AOUT_OK;

error:
        assert(packet); assert(packet->referenceCount == 0);
        enet_packet_destroy(packet);

        return AOUT_ERR;
}

aout_res aout_host_create_packet(
                size_t size,
                uint32_t flags,
                ENetPacket** out_packet,
                aout_stream* out_stream) {
        assert(out_packet); assert(out_stream);

        *out_packet = enet_packet_create(
                NULL,
                sizeof(aout_msg_type) + size,
                flags
        );

        if (!*out_packet) {
                return AOUT_ERR;
        }

        *out_stream = (aout_stream) {
                .data = (*out_packet)->data,
                .data_size = (*out_packet)->dataLength,
        };

        return AOUT_OK;
}
