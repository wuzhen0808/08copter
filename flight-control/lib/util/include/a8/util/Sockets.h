#pragma once
#include "a8/util/Callback2.h"
#include "a8/util/Result.h"
#include "a8/util/String.h"
namespace a8::util {

typedef int SOCK;

class Sockets {

public:
    
    // virtual Result setBlocking(SOCK sock, bool blocking) = 0;

    virtual bool send(SOCK sock, const char * buf, int len) = 0;

    virtual int receive(SOCK sock, char *buf, int bufLen) = 0;

    virtual int getLastError() = 0;

    virtual Result connect(SOCK sock, const String host, int port) = 0;

    virtual Result bind(SOCK sock, const String address, int port) = 0;

    virtual Result listen(SOCK sock) = 0;

    virtual SOCK accept(SOCK sock) = 0;

    virtual void close(SOCK &sock) = 0;

    virtual SOCK socket() = 0;

    virtual int select(Buffer<SOCK> &readSockets, Buffer<SOCK> &writeSockets, Buffer<SOCK> &exceptionSockets) = 0;

    virtual int select(SOCK &sock) = 0;
};

} // namespace a8::util