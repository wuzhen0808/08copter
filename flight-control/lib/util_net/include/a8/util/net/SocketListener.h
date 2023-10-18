#pragma once
#include "a8/util/net/Sockets.h"

namespace a8::util::net {

class SocketListener {
public:
    virtual void onConnection(Sockets *sockets, SOCK sock) = 0;
};

} // namespace a8::util::net