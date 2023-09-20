#include "a8/util/String.h"

namespace a8::hal::socket {
using a8::util::String;

class Connection {
public:
    virtual Connection *connect(String host, int port) = 0;

    virtual void close() = 0;

    virtual int send(const char *buf, int size) = 0;

    virtual void receive() = 0;
};

class Server {
};

} // namespace a8::hal::socket