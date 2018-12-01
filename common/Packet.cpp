#include "Packet.hpp"
#include "Endian.hpp"

#include <enet/enet.h>
#include <cstring>

namespace aout {

Packet::Packet(Flag flag)
        : mFlag(flag)
        , mReadPosition(0)
        , mIsValid(true) {
}

Packet::~Packet() = default;

Packet& Packet::append(const void* data, std::size_t size) {
        if (data != nullptr && size > 0) {
                std::size_t start = mData.size();
                mData.resize(start + size);
                std::memcpy(&mData[start], data, size);
        }

        return *this;
}

void Packet::clear() {
        mData.clear();
        mReadPosition = 0;
        mIsValid = true;
}

bool Packet::dataLeft() const {
        return mReadPosition < mData.size();
}

bool Packet::isValid() const {
        return mIsValid;
}

Packet::Flag Packet::getFlag() const {
        return mFlag;
}

const uint8* Packet::getData() const {
        return mData.empty() ? nullptr : &mData[0];
}

std::size_t Packet::getSize() const {
        return mData.size();
}

Packet::operator bool() const {
        return isValid();
}

const Packet& Packet::operator>>(bool& data) const {
        uint8 value;
        if (*this >> value) {
                data = (value != 0);
        }

        return *this;
}

const Packet& Packet::operator>>(int8& data) const {
        if (checkSize(sizeof(data))) {
                data = *reinterpret_cast<const int8*>(&mData[mReadPosition]);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(uint8& data) const {
        if (checkSize(sizeof(data))) {
                data = *reinterpret_cast<const uint8*>(&mData[mReadPosition]);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(int16& data) const {
        if (checkSize(sizeof(data))) {
                int16 read = *reinterpret_cast<const int16*>(&mData[mReadPosition]);
                data = util::ntoh16(read);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(uint16& data) const {
        if (checkSize(sizeof(data))) {
                uint16 read = *reinterpret_cast<const uint16*>(&mData[mReadPosition]);
                data = util::ntoh16(read);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(int32& data) const {
        if (checkSize(sizeof(data))) {
                int32 read = *reinterpret_cast<const int32*>(&mData[mReadPosition]);
                data = util::ntoh32(read);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(uint32& data) const {
        if (checkSize(sizeof(data))) {
                uint32 read = *reinterpret_cast<const uint32*>(&mData[mReadPosition]);
                data = util::ntoh32(read);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(int64& data) const {
        if (checkSize(sizeof(data))) {
                int64 read = *reinterpret_cast<const int64*>(&mData[mReadPosition]);
                data = util::ntoh64(read);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(uint64& data) const {
        if (checkSize(sizeof(data))) {
                uint64 read = *reinterpret_cast<const uint64*>(&mData[mReadPosition]);
                data = util::ntoh64(read);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(float32& data) const {
        static_assert(sizeof(float32) == sizeof(uint32), "Both have same size");

        uint32 read;
        if (*this >> read) {
                data = util::unpackIEC559_32(read);
        }

        return *this;
}

const Packet& Packet::operator>>(float64& data) const {
        static_assert(sizeof(float64) == sizeof(uint64), "Both have same size");

        uint64 read;
        if (*this >> read) {
                data = util::unpackIEC559_64(read);
        }

        return *this;
}

const Packet& Packet::operator>>(std::string& data) const {
        // Extract the string length (limited to 256!)
        uint16 length = 0;
        *this >> length;

        if (length > 0 && checkSize(length)) {
                const char* str = reinterpret_cast<const char*>(&mData[mReadPosition]);
                data.assign(str, length);
                mReadPosition += length;
        }

        return *this;
}

Packet& Packet::operator<<(bool data) {
        return *this << static_cast<uint8>(data);
}

Packet& Packet::operator<<(int8 data) {
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(uint8 data) {
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(int16 data) {
        data = util::hton16(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(uint16 data) {
        data = util::hton16(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(int32 data) {
        data = util::hton32(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(uint32 data) {
        data = util::hton32(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(int64 data) {
        data = util::hton64(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(uint64 data) {
        data = util::hton64(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(float32 data) {
        static_assert(sizeof(float32) == sizeof(uint32), "Both have same size");

        uint32 encoded = util::packIEC559_32(data);
        return *this << encoded;
}

Packet& Packet::operator<<(float64 data) {
        static_assert(sizeof(float64) == sizeof(uint64), "Both have same size");

        uint64 encoded = util::packIEC559_64(data);
        return *this << encoded;
}

Packet& Packet::operator<<(const std::string& data) {
        // Insert string length (must be < 256!)

        if (data.size() > 0 && data.size() < 256) {
                uint16 length = data.size();

                if (*this << length) {
                        append(data.c_str(), length  * sizeof(std::string::value_type));
                }
        }

        return *this;
}

bool Packet::checkSize(std::size_t size) const {
        mIsValid = mIsValid && (mReadPosition + size <= mData.size());
        return mIsValid;
}

void Packet::convertFrom(_ENetPacket& enetPacket) {
        clear();
        append(enetPacket.data, enetPacket.dataLength);
}

}
