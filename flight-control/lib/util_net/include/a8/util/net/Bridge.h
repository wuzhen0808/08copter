#pragma once
#include "a8/util.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {
using namespace a8::util;

template <typename T>
class Bridge : public FlyWeight {
    using skeletonRelease = void (*)(T *);

    bridge bridge_;
    skeletonRelease skeletonRelease_;
    T *skeleton;
    Channel *channel;
    //
    void *stub_;
    void (*stubRelease_)(void *);
    //
    bool running = true;
    int ret = -1;
    Result rst;

public:
    Bridge(bridge bridge, T *skeleton, Channel *channel, LoggerFactory *logF) : FlyWeight(logF) {
        this->bridge_ = bridge;
        this->skeleton = skeleton;
        this->channel = channel;
        this->stub_ = 0;
        this->stubRelease_ = 0;
    }
    ~Bridge() {
        if (this->stub_ != 0) {
            this->stubRelease_(this->stub_);
        }
        Lang::free<T>(skeleton);
        delete this->channel;
    }

    int isRunning(Result &rst) {
        if (this->running) {
            return 1;
        }
        rst = this->rst;
        return -1;
    }
    void close() {
        this->channel->close();
    }

    template <typename B>
    B *stub() {
        return Lang::cast<B *>(this->stub_);
    }

    template <typename B>
    B *createStub(B *(*stubCreate)(Channel *), void (*stubRelease)(void *)) {
        if (this->stub_ != 0) {
            this->stubRelease_(stub_);
        }
        this->stubRelease_ = stubRelease;
        B * stub = stubCreate(this->channel);
        this->stub_ = Lang::cast<B *>(stub);
        return stub;
    }

    Channel *getChannel() {
        return channel;
    }

    void run() {

        log("running Bridge.");
        ret = channel->receive(bridge_, skeleton, -1, rst);
        if (ret < 0) {
            log(String() << "Error while receiving data from channel," << rst);
        }
        this->running = false;
        log("Bridge exited.");
    }
};

} // namespace a8::util::net
