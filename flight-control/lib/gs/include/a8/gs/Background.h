#pragma once
#include "a8/gs/GsSkeleton.h"
#include "a8/gs/defines.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
namespace a8::gs {
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;

using bridgeHandle = void (*)(Bridge<GsSkeleton> *, int, void *); // bridge,eventType,context.

class Background : public FlyWeight {

    static GsSkeleton *createSkeleton(Background *bg) {
        return new GsSkeleton(bg->loggerFactory);
    }

    FcApi *fcApi;
    BridgeKeeper<GsSkeleton, FcStub> *bridgeKeeper;

public:
    Background(Links *links, EventCenter *eventCenter, LoggerFactory *logFac) : FlyWeight(logFac) {

        this->bridgeKeeper = new BridgeKeeper<GsSkeleton, FcStub>(links->gsAddress());
        this->bridgeKeeper->setEventCenter(eventCenter);
        this->bridgeKeeper->setEventTypeOfAfterBridgeCreate(EventTypes::AFTER_BRIDGE_CREATE_);
        this->bridgeKeeper->setEventTypeOfBeforeBridgeFree(EventTypes::BEFORE_BRIDGE_FREE_);
        eventCenter->add<Bridge<GsSkeleton> *, Background *>(
            EventTypes::AFTER_BRIDGE_CREATE_,
            this,
            [](Bridge<GsSkeleton> *bridge, Background *this_) {
                this_->afterBridgeCreate(bridge);
            });
        eventCenter->add<Bridge<GsSkeleton> *, Background *>(
            EventTypes::BEFORE_BRIDGE_FREE_,
            this,
            [](Bridge<GsSkeleton> *bridge, Background *this_) {
                this_->beforeBridgeFree(bridge);
            });
        eventCenter->add<void *, Background *>(
            EventTypes::BEFORE_QUIT,
            this,
            [](void *zero, Background *this_) {
                this_->close();
            });
    }

    void beforeBridgeFree(Bridge<GsSkeleton> *bridge) {
        log("beforeBridgeFree");
    }

    void afterBridgeCreate(Bridge<GsSkeleton> *bridge) {
        log("afterBridgeCreate.");
    }

    int open(Result &rst) {
        // waiting fcs to connect
        int ret = bridgeKeeper->bindAndListen(rst);
        log("listening connect in on port of gs.");
        return ret;
    }

    void close() {
        bridgeKeeper->close();
    }

    BridgeKeeper<GsSkeleton, FcStub> *getBridgeKeeper() {
        return this->bridgeKeeper;
    }

    int run(TickingContext *ticking) {
        log("GS net accepter running...");
        Result rst;
        int ret = bridgeKeeper->run(createSkeleton, FcStub::create, this, rst);
        log(String() << "Warning: GS main loop exit.");
        return ret;
    }
};

} // namespace a8::gs