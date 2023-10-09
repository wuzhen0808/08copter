#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {

class Channel {

    Sockets *sockets;
    SOCK sock;
    Codec *codecs_;
    //
    SocketWriter *writer;
    SocketReader *reader;
    int lastResult = 0;
    // codec context
    void *codecObj;

public:
    Channel(Sockets *sockets, SOCK sock, Codec *codecs) {
        this->sockets = sockets;
        this->sock = sock;
        this->codecs_ = codecs;
        this->reader = new SocketReader(sockets, sock);
        this->writer = new SocketWriter(sockets, sock);
    }
    ~Channel() {
        delete this->reader;
        delete this->writer;
    }

    void close() {
        this->sockets->close(sock);
    }

    int send(int type, void *data, Result &rst) {
        int ret = codecs_->write(writer, type, data, rst);
        if (ret < 0) {
            return ret;
        }
        return 1;
    }

    int receive(bridge bridgeF, void *skeleton, Result &rst) {
        return receive(bridgeF, skeleton, 1, rst);
    }

    int receiveAll(bridge bridgeF, void *skeleton, Result &rst) {
        return receive(bridgeF, skeleton, -1, rst);
    }

    int receive(bridge bridgeF, void *skeleton, int len, Result &rst) {

        int received = 0;
        while ((len == -1 || received < len) && lastResult >= 0) {
            
            lastResult = codecs_->read(reader, bridgeF, skeleton, rst);
            if (lastResult <= 0) {                
                break;
            }
            received++;
        }

        return received == 0 ? lastResult : received;
    }
};

} // namespace a8::util::net
