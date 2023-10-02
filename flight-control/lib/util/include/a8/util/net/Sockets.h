#pragma once
#include "a8/util.h"
namespace a8::util::net {

typedef int SOCK;

class Sockets {

public:
    virtual int socket(SOCK & sock) = 0;

    virtual SOCK socket() = 0;//TODO remove.

    virtual bool send(SOCK sock, const char *buf, int len) = 0;

    virtual int receive(SOCK sock, char *buf, int bufLen) = 0;

    virtual int getLastError() = 0;

    virtual Result connect(SOCK sock, const String host, int port) = 0;

    virtual int bind(SOCK sock, const String address, int port) = 0;

    virtual int listen(SOCK sock) = 0;

    virtual int accept(SOCK sock, SOCK &sockIn) = 0;
    
    virtual SOCK accept(SOCK sock) = 0;

    virtual void close(SOCK &sock) = 0;
   

    virtual int select(Buffer<SOCK> &readSockets, Buffer<SOCK> &writeSockets, Buffer<SOCK> &exceptionSockets) = 0;

    virtual int select(SOCK &sock) = 0;
};

} // namespace a8::util