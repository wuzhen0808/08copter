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
    using stubCreate = void *(Channel *);

    bridge bridge_;
    skeletonRelease skeletonRelease_;
    void *skeleton;
    Channel *channel;
    //
    void *stub_;
    stubRelease stubRelease_;
    //
    bool running = true;
    int ret = -1;
    Result rst;

public:
    Bridge(bridge bridge, void *skeleton, skeletonRelease skeletonRelease, Channel *channel, LoggerFactory *logF) : FlyWeight(logF) {
        this->bridge_ = bridge;
        this->skeleton = skeleton;
        this->skeletonRelease_ = skeletonRelease;
        this->channel = channel;
        this->stub_ = 0;
        this->stubRelease_ = 0;
    }
    ~Bridge() {
        if (this->stub_ != 0) {
            this->stubRelease_(this->stub_);
        }
        skeletonRelease_(this->skeleton);
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

    template <typename T>
    T *stub() {
        return static_cast<T *>(this->stub_);
    }
    template <typename T>
    T *createStub(stubCreate create, stubRelease release) {
        if (this->stub_ != 0) {
            this->stubRelease_(stub_);            
        }
        this->stubRelease_ = release;
        this->stub_ = create(this->channel);
        return static_cast<T *>(this->stub_);
    }

    Channel *getChannel() {
        return channel;
    }

    void run() {

        log("running Bridge.");
        ret = channel->receive(bridge_, skeleton, -1, rst);
        if (ret < 0) {
            // todo log.
        }
        this->running = false;
        log("Bridge exited.");
    }
};

} // namespace a8::util::net
