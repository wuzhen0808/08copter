#pragma once
#include "a8/util/net/Address.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Bridge.h"
#include "a8/util/net/SimpleCodec.h"
#include "a8/util/thread.h"

using namespace a8::util::thread;

namespace a8::util::net {

/**
 *  Network is a utility wrapper on top of socket.
 *
 */
class Network {

    Sockets *sockets;
    Codec *codec;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

public:
    Network(Sockets *sockets, Codec *codec, Scheduler *scheduler, LoggerFactory *loggerFactory) {
        this->sockets = sockets;
        this->codec = codec;
        this->scheduler = scheduler;
        this->loggerFactory = loggerFactory;
    }

    Address * address(bridge bridgeF, String host, int port) {
        return new Address(this->sockets, this->codec, bridgeF, host, port, this->scheduler, this->loggerFactory);
    }
};

} // namespace a8::util::net
