#pragma once
#include "a8/util/Sockets.h"

namespace a8::util {

class SocketConnector {
public:
    virtual void onConnection(Sockets *sockets, SOCK sock) = 0;
};

} // namespace a8::util