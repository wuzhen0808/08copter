#pragma once
#include "a8/util.h"
namespace a8::util::net {

typedef int SOCK;

class Sockets {

public:
    virtual int socket(SOCK &sock) = 0;

    virtual bool send(SOCK sock, const char *buf, int len, Result &res) = 0;

    virtual int receive(SOCK sock, char *buf, int bufLen, Result &res) = 0;

    virtual int getLastError() = 0;

    virtual int connect(SOCK sock, const String host, int port, Result &rst) = 0;

    virtual int bind(SOCK sock, const String address, int port, Result &res) = 0;

    virtual int listen(SOCK sock, Result &rst) = 0;

    virtual int accept(SOCK sock, SOCK &sockIn, Result &res) = 0;

    virtual int close(SOCK sock) = 0;

    virtual int select(Buffer<SOCK> &readSockets, Buffer<SOCK> &writeSockets, Buffer<SOCK> &exceptionSockets) = 0;

    virtual int select(SOCK &sock) = 0;
};

} // namespace a8::util::net