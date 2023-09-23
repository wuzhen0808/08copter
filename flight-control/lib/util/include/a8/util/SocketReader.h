#pragma once
#include "a8/util/Logger.h"
#include "a8/util/Reader.h"
#include "a8/util/Runnable.h"
#include "a8/util/Socket.h"
#include "a8/util/String.h"

using namespace a8::util;
namespace a8::util {

class SocketReader : public Reader {
private:
    Socket *socket;
public:
    SocketReader(Socket *socket) {
        this->socket = socket;
    }

    ~SocketReader() {
    }

    virtual int read(char *buf, int bufLen) {
        return socket->receive(buf, bufLen);
    }
};
} // namespace a8::util