#include "Packet.hpp"

#include <enet/enet.h>
#include <cstring>

namespace aout { namespace net {

Packet::Packet(Flag flag)
        : mFlag(flag)
        , mReadPosition(0)
        , mIsValid(true) {
}

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
                data = ntohs(*reinterpret_cast<const int16*>(&mData[mReadPosition]));
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(uint16& data) const {
        if (checkSize(sizeof(data))) {
                data = ntohs(*reinterpret_cast<const uint16*>(&mData[mReadPosition]));
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(int32& data) const {
        if (checkSize(sizeof(data))) {
                data = ntohl(*reinterpret_cast<const int32*>(&mData[mReadPosition]));
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(uint32& data) const {
        if (checkSize(sizeof(data))) {
                data = ntohl(*reinterpret_cast<const uint32*>(&mData[mReadPosition]));
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(float& data) const {
        if (checkSize(sizeof(data))) {
                data = *reinterpret_cast<const float*>(&mData[mReadPosition]);
                mReadPosition += sizeof(data);
        }

        return *this;
}

const Packet& Packet::operator>>(double& data) const {
        if (checkSize(sizeof(data))) {
                data = *reinterpret_cast<const double*>(&mData[mReadPosition]);
                mReadPosition += sizeof(data);
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
        data = htons(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(uint16 data) {
        data = htons(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(int32 data) {
        data = htonl(data);
        return append(&data, sizeof(data));
}

Packet& Packet::operator<<(uint32 data) {
        data = htonl(data);
        return append(&data, sizeof(data));
}

// TODO: Take byte order into consideration
Packet& Packet::operator<<(float data) {
        return append(&data, sizeof(data));
}

// TODO: Take byte order into consideration
Packet& Packet::operator<<(double data) {
        return append(&data, sizeof(data));
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

} }
