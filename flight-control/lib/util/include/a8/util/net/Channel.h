#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Address.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/Dispatcher.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {

class Channel {

    Sockets *sockets;
    Address *address;
    SOCK sock;
    Codec *codecs;
    Dispatcher *dispatcher;

    SocketWriter *writer;
    SocketReader *reader;
    int lastResult = 0;

public:
    Channel(Sockets *sockets, Address *address, SOCK sock, Codec *codec, Dispatcher *dispatcher) {
        this->sockets = sockets;
        this->address = address;
        this->sock = sock;
        this->codecs = codecs;
        this->dispatcher = dispatcher;
        this->reader = new SocketReader(sockets, sock);
        this->writer = new SocketWriter(sockets, sock);
    }

    int send(int type, void *data) {
        int rst = codecs->write(writer, type, data);
        if (rst < 0) {
            return rst;
        }
        return 1;
    }

    int receive() {
        return receive(1);
    }

    int receive(int len) {
        int received = 0;
        while (received < len && lastResult >= 0) {
            int type = 0;
            void *data = 0;
            lastResult = codecs->read(reader, type, data);

            if (lastResult <= 0) {
                break;
            }

            dispatcher->dispatch(type, data, this->address->receiver);
            received++;
        }

        return received == 0 ? lastResult : received;
    }
};

} // namespace a8::util::net
