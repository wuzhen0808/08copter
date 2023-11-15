#pragma once
#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;

class Rf24ConnectResponse {

public:
    static int read(Reader *reader, Rf24ConnectResponse &data) {
        int len = 0;
        int ret = CodecUtil::readInt<int>(reader, data.node1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::readInt<int>(reader, data.port1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::readInt<int>(reader, data.node2);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::readInt<int>(reader, data.port2);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }
    static int write(Writer *writer, Rf24ConnectResponse &data) {
        int len = 0;
        int ret = CodecUtil::writeInt<int>(writer, data.node1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::writeInt<int>(writer, data.port1);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::writeInt<int>(writer, data.node2);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        ret = CodecUtil::writeInt<int>(writer, data.port2);
        if (ret < 0) {
            return ret;
        }
        len += ret;

        return len;
    }

    int node1; // client node id.
    int port1; // client port.
    int node2; // target node id;
    int port2; // target port
    Rf24ConnectResponse() {
    }
    ~Rf24ConnectResponse() {
    }
};

} // namespace a8::hal::rf24