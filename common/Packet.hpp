#pragma once

#include "Config.hpp"

#include <vector>

struct _ENetPacket;

namespace aout {

class AOUT_API Packet final {
public:
        enum class Flag : uint16 {
                Unreliable,
                Sequenced,
                Reliable,
        };

public:
        explicit Packet(Flag flag = Flag::Unreliable);
        ~Packet() = default;

        Packet& append(const void* data, std::size_t size);
        void clear();

        bool dataLeft() const;
        bool isValid() const;

        Flag getFlag() const;
        const uint8* getData() const;
        std::size_t getSize() const;

        explicit operator bool() const;

        const Packet& operator>>(bool& data) const;
        const Packet& operator>>(int8& data) const;
        const Packet& operator>>(uint8& data) const;
        const Packet& operator>>(int16& data) const;
        const Packet& operator>>(uint16& data) const;
        const Packet& operator>>(int32& data) const;
        const Packet& operator>>(uint32& data) const;
        const Packet& operator>>(int64& data) const;
        const Packet& operator>>(uint64& data) const;
        const Packet& operator>>(float32& data) const;
        const Packet& operator>>(float64& data) const;
        const Packet& operator>>(std::string& data) const;

        Packet& operator<<(bool data);
        Packet& operator<<(int8 data);
        Packet& operator<<(uint8 data);
        Packet& operator<<(int16 data);
        Packet& operator<<(uint16 data);
        Packet& operator<<(int32 data);
        Packet& operator<<(uint32 data);
        Packet& operator<<(int64 data);
        Packet& operator<<(uint64 data);
        Packet& operator<<(float32 data);
        Packet& operator<<(float64 data);
        Packet& operator<<(const std::string& data);

private:
        bool checkSize(std::size_t size) const;
        void convertFrom(_ENetPacket& enetPacket);

private:
        std::vector<uint8> mData;
        mutable std::size_t mReadPosition;
        mutable bool mIsValid;
        Flag mFlag;

        friend class Host;
};

}
