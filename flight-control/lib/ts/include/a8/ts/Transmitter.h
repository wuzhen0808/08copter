#pragma once
#include "a8/common.h"
#include "a8/hal.h"
#include "a8/ts/TsSkeleton.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include "a8/util/schedule.h"

namespace a8::ts {
using namespace a8::util;
using namespace a8::util::schedule;
using namespace a8::util::comp;
using namespace a8::common;

class Transmitter : public Component {
    static const int TICKING_KEY_BRIDGE = 0;

protected: // fields
    int port;
    int argc;
    char **argv;
    //
    Links *links;
    BridgeKeeper<TsSkeleton, FcStub> *bridgeKeeperFc;

protected: // functions
    static TsSkeleton *createSkeleton(Transmitter *this_) {
        this_->log("createSkeleton.");

        return new TsSkeleton(this_->loggerFactory);
    }

    // TOTO do not actively sending data to gs.
    // only reactive to the command from gs.
    // processing command from gs.

    void init(int argc, char **argv, Links *links) {
        this->links = links;
        this->argc = argc;
        this->argv = argv;
    }

public:
    Transmitter(const String &name, int argc, char **argv, Links *links) : Component(name) {
        this->init(argc, argv, links);
    }

    ~Transmitter() {
    }

    void boot(StagingContext *context) override {

        CommonUtil::resolveProperties(argc, argv, context->properties, context->getSys());
        Component::boot(context);
        this->bridgeKeeperFc = new BridgeKeeper<TsSkeleton, FcStub>(this->links->fcAddress(), context->loggerFactory);
        this->schedule<Transmitter>(1.0f, [](TickingContext *ticking, Transmitter *this_) {
            this_->processFcCommands(ticking);
        }); //
    }

    void processFcCommands(TickingContext *ticking) {
        log(">>processFcCommands");
        Bridge<TsSkeleton> *fcBridge = 0;
        ticking->ret = this->bridgeKeeperFc->get(fcBridge, Transmitter::createSkeleton, FcStub::create, this, ticking->rst);
        //ticking->ret = -1;
        //ticking->rst << "test-error.";
        if (ticking->ret < 0) {
            log("ticking->ret<0,err:" << ticking->rst.errorMessage);
            return;
        }
        log("getStub>>");
        // connected already.
        FcApi *fcApi = fcBridge->stub<FcApi>();
        ticking->ret = fcApi->ping("hello fc, this is ts.", ticking->rst);
        if (ticking->ret < 0) {
            return;
        }
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
        //
    }

    void setup(StagingContext *context) override {
        Component::setup(context);
    }
    void start(StagingContext *context) override {
        Component::start(context);
    }

    void test() {
    }
};

} // namespace a8::ts