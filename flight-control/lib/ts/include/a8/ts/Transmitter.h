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
        return new TsSkeleton(this_->loggerFactory);
    }

    // TOTO do not actively sending data to gs.
    // only reactive to the command from gs.
    // processing command from gs.

    void init(int argc, char **argv, Links *links) {
        this->links = links;
        this->argc = argc;
        this->argv = argv;
        this->bridgeKeeperFc = new BridgeKeeper<TsSkeleton, FcStub>(this->links->gsAddress());
    }

public:
    Transmitter(const String &name, int argc, char **argv, Links *links) : Component(name) {
        this->init(argc, argv, links);
    }

    ~Transmitter() {
    }

    virtual void boot(StagingContext *context) override {

        CommonUtil::resolveProperties(argc, argv, context->properties, context->getSys());
        Component::boot(context);
    }

    void processGsCommands(TickingContext *ticking) {
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
        //
    }

    virtual void setup(StagingContext *context) override {
        Component::setup(context);
    }
    virtual void start(StagingContext *context) override {
        Component::start(context);
    }

    void test() {
    }
};

} // namespace a8::ts