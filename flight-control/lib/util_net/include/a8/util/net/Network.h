#pragma once
#include "a8/util/net/Address.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Bridge.h"
#include "a8/util/net/SimpleCodec.h"
#include "a8/util/schedule.h"

using namespace a8::util::schedule;

namespace a8::util::net {

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Network {

    Sockets *sockets;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

public:
    Network(Sockets *sockets, Scheduler *scheduler, LoggerFactory *loggerFactory) {
        this->sockets = sockets;
        this->scheduler = scheduler;
        this->loggerFactory = loggerFactory;
    }

    Address * address(bridge bridgeF, String host, int port, Codec *codec) {
        return new Address(this->sockets, codec, bridgeF, host, port, this->scheduler, this->loggerFactory);
    }
};

} // namespace a8::util::net
