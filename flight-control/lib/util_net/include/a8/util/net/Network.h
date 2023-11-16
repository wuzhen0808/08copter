#pragma once
#include "a8/util/net/Address.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Bridge.h"
#include "a8/util/net/SimpleCodec.h"
#include "a8/util/sched.h"


namespace a8::util::net {
using namespace a8::util::sched;

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Network {

    Sockets *sockets;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

public:
    Address * address(bridge bridgeF, String host, int port, Codec *codec) {
        return new Address(this->sockets, codec, bridgeF, host, port, this->scheduler, this->loggerFactory);
    }

    Network(Sockets *sockets, Scheduler *scheduler, LoggerFactory *loggerFactory) {
        this->sockets = sockets;
        this->scheduler = scheduler;
        this->loggerFactory = loggerFactory;
    }

};

} // namespace a8::util::net
