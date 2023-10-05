#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Address.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {

class Channel {

    Sockets *sockets;
    Address *address_;
    SOCK sock;
    Codec *codecs_;
    FuncType::handle handle_;
    void *context_;
    //
    SocketWriter *writer;
    SocketReader *reader;
    int lastResult = 0;

public:
    Channel(Sockets *sockets, Address *address, SOCK sock, Codec *codecs, FuncType::handle handle, void *context) {
        this->sockets = sockets;
        this->address_ = address;
        this->sock = sock;
        this->codecs_ = codecs;
        this->handle_ = handle;
        this->context_ = context;
        this->reader = new SocketReader(sockets, sock);
        this->writer = new SocketWriter(sockets, sock);
    }

    int send(int type, void *data, Result& rst) {
        int ret = codecs_->write(writer, type, data, rst);
        if (ret < 0) {
            return ret;
        }
        return 1;
    }

    int receive(Result & rst) {
        return receive(1, rst);
    }

    int receive(int len, Result& rst) {
        int received = 0;
        while (received < len && lastResult >= 0) {
            int type = 0;
            void *data = 0;
            lastResult = codecs_->read(reader, type, data, rst);

            if (lastResult <= 0) {
                break;
            }
            handle_(type, data, context_);
            
            received++;
        }

        return received == 0 ? lastResult : received;
    }
};

} // namespace a8::util::net
