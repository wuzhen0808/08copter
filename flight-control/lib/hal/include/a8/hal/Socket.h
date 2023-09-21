#include "a8/util/String.h"

namespace a8::hal::socket {
using a8::util::String;

class Socket;

class SocketFactory {
public:
    virtual Socket *socket() = 0;
};

class Socket {

public:
    virtual bool connect(String host, int port) = 0;

    virtual bool setBlocking(bool blocking) = 0;

    virtual bool send(String * str) = 0;

    virtual int receive(char *buf, int bufLen) = 0;
};

class Server {
};

} // namespace a8::hal::socket