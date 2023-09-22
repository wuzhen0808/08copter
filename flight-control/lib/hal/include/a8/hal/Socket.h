#pragma once
#include "a8/util/String.h"
#include "a8/util/Result.h"

namespace a8::hal::socket {
using a8::util::String;
using a8::util::Result;

class Socket {

public:
    virtual Result connect(String host, int port) = 0;

    virtual bool setBlocking(bool blocking) = 0;

    virtual bool send(String * str) = 0;

    virtual int receive(char *buf, int bufLen) = 0;

    virtual int getLastError() = 0;
};

class SocketFactory {
public:
    virtual Socket *socket() = 0;
};

class Server {
};

} // namespace a8::hal::socket