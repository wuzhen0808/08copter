#pragma once
#include "a8/util/comp.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

using namespace a8::util::comp;
namespace a8::util::net {

class Bridge : public FlyWeight {
    using skeletonRelease = void (*)(void *);
    using stubRelease = void (*)(void *);

    bridge bridge_;
    skeletonRelease skeletonRelease_;    
    void *skeleton;
    Channel *channel;

public:
    Bridge(bridge bridge, void *skeleton, skeletonRelease skeletonRelease, Channel *channel, LoggerFactory *logF) : FlyWeight(logF) {
        this->bridge_ = bridge;
        this->skeleton = skeleton;
        this->skeletonRelease_ = skeletonRelease;            
        this->channel = channel;
    }
    ~Bridge() {
        skeletonRelease_(this->skeleton);
        delete this->channel;
    }

    Channel * getChannel(){
        return channel;
    }

    void run() {
        Result rst;
        log("running Bridge.");
        int ret = channel->receive(bridge_, skeleton, -1, rst);
        if (ret < 0) {
            // todo log.
        }
        log("Bridge exited.");
    }
};

} // namespace a8::util::net
