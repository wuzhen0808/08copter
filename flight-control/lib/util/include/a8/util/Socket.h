#pragma once
#include "a8/util/Result.h"
#include "a8/util/String.h"

namespace a8::util {

class Socket {

public:
    virtual bool setBlocking(bool blocking) = 0;

    virtual bool send(String *str) = 0;

    virtual int receive(char *buf, int bufLen) = 0;

    virtual int getLastError() = 0;
};

class SocketClient : virtual public Socket {
public:
    virtual Result connect(String host, int port) = 0;
};

class SocketServer : virtual public Socket {
public:
    virtual Result bind(String address, int port) = 0;
    virtual Result listen() = 0;
    virtual Socket *accept() = 0;
};

class SocketFactory {
public:
    virtual SocketClient *socketClient() = 0;
    virtual SocketServer *socketServer() = 0;
};

} // namespace a8::hal::socket