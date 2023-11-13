#pragma once
#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;

class Rf24ConnectRequest {

public:
    static int read(Reader *reader, Rf24ConnectRequest &data) {
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

        return len;
    }
    static int write(Writer *writer, Rf24ConnectRequest &data) {
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

        return len;
    }

    int node1; // client node id.
    int port1; // client port.
    Rf24ConnectRequest() {
    }
    ~Rf24ConnectRequest() {
    }
};

} // namespace a8::hal::rf24