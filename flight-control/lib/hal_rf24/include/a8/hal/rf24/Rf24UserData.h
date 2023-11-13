#pragma once
#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;

class Rf24UserData {

public:
    static int read(Reader *reader, Rf24UserData &data) {
        int len = 0;
        int ret = CodecUtil::readInt<int>(reader, data.sockId);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        int size = 0;
        ret = CodecUtil::readInt<int>(reader, size);
        if (ret < 0) {
            return ret;
        }
        len += ret;
        
        char buf[size];
        ret = reader->read(buf, size);
        data.buffer->append(buf, size);

        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }
    static int write(Writer *writer, Rf24UserData &data) {
        int len = 0;
        int ret = CodecUtil::writeInt<int>(writer, data.sockId);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::writeInt<int>(writer, data.buffer->len());
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = writer->write(data.buffer->buffer(), data.buffer->len());
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }

    int sockId; // sockId,channel id.
    Buffer<char> *buffer;
    Rf24UserData() {
        this->buffer = new Buffer<char>();
    }
    ~Rf24UserData() {
        delete this->buffer;
    }
};

} // namespace a8::hal::rf24