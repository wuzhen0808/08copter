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

template <typename K, typename S>
class Channel {
    using stubCreate = S *(*)(Channel *);
    using stubRelease = void (*)(S *);

    stubCreate stubCreate_;
    stubRelease stubRelease_;
    K *skeleton;
    S *stub_ = 0;

    Sockets *sockets;
    SOCK sock;
    Codec *codecs_;
    bridge bridge_;
    //
    SocketWriter *writer;
    SocketReader *reader;
    int lastResult = 0;

public:
    Channel(Sockets *sockets, SOCK sock, Codec *codecs, bridge bridge, K *skeleton, stubCreate stubCreate_, stubRelease stubRelease_) {
        this->sockets = sockets;
        this->sock = sock;
        this->codecs_ = codecs;
        this->bridge_ = bridge;
        this->skeleton = skeleton;
        this->stubCreate_ = stubCreate_;
        this->stubRelease_ = stubRelease_;
        this->stub_ = stubCreate_(this);
        this->reader = new SocketReader(sockets, sock);
        this->writer = new SocketWriter(sockets, sock);
    }
    ~Channel() {
        delete this->reader;
        delete this->writer;
        stubRelease_(this->stub_);
    }

    S *getStub() {
        return this->stub_;
    }

    int send(int type, void *data, Result &rst) {
        int ret = codecs_->write(writer, type, data, rst);
        if (ret < 0) {
            return ret;
        }
        return 1;
    }

    int receive(Result &rst) {
        return receive(1, rst);
    }

    int receive(int len, Result &rst) {
        if (this->bridge_ == 0) {
            return -1;
        }
        int received = 0;
        while (received < len && lastResult >= 0) {
            int type = 0;
            void *data = 0;
            lastResult = codecs_->read(reader, type, data, rst);

            if (lastResult <= 0) {
                break;
            }
            bridge_(type, data, skeleton);

            received++;
        }

        return received == 0 ? lastResult : received;
    }
};

} // namespace a8::util::net
