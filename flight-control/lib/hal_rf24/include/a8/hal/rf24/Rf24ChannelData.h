#pragma once
#include "a8/util/net.h"

namespace a8::hal::nrf24 {
using namespace a8::util;
class Rf24ChannelData {

public:
    static int read(Reader *reader, Rf24ChannelData &data) {
        int len = 0;
        int ret = CodecUtil::readInt<int>(reader, data.channel);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        data.buffer = new char[data.size];
        ret = reader->read(data.buffer, data.size);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }
    static int write(Writer *writer, Rf24ChannelData &data) {
        int len = 0;
        int ret = CodecUtil::writeInt<int>(writer, data.channel);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        ret = writer->write(data.buffer, data.size);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }

    int channel; // channel id, represent a connection .
    int size;    // pack size.
    char *buffer;
    Rf24ChannelData() {
    }
    ~Rf24ChannelData() {
        if (this->buffer != 0) {
            delete[] this->buffer;
        }
    }
};

} // namespace a8::hal::nrf24