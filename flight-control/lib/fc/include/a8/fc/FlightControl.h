#pragma once
#include "a8/common.h"
#include "a8/fc/FcSkeleton.h"
#include "a8/fc/defines.h"
#include "a8/hal.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include "a8/util/sched.h"

namespace a8::fc {
using namespace a8::util;
using namespace a8::util::sched;
using namespace a8::util::comp;
using namespace a8::common;
class FlightControl : public Component {
    static const int TICKING_KEY_BRIDGE = 0;

protected: // fields
    int totalServos_;
    String propertiesFile;
    String host;
    int port;
    int argc;
    char **argv;
    //
    Links *links;
    BridgeKeeper<FcSkeleton, GsStub> *bridgeKeeperGs;
    BridgeKeeper<FcSkeleton, TsStub> *bridgeKeeperTs;

protected: // functions
    static FcSkeleton *createSkeleton(FlightControl *this_) {
        return new FcSkeleton(this_->loggerFactory);
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
    FlightControl(const String &name, int argc, char **argv, Links *links) : Component(name) {
        this->init(argc, argv, links);
        this->totalServos_ = 4;
    }
    FlightControl(const String &name, int argc, char **argv, Links *links, int totalServos) : Component(name) {
        this->init(argc, argv, links);
        this->totalServos_ = totalServos;
    }

    ~FlightControl() {
    }

    virtual void boot(StagingContext *context) override {

        CommonUtil::resolveProperties(argc, argv, context->properties, context->getSys());
        Component::boot(context);
        this->bridgeKeeperTs = new BridgeKeeper<FcSkeleton, TsStub>(this->links->gsAddress(), context->loggerFactory);
        this->bridgeKeeperGs = new BridgeKeeper<FcSkeleton, GsStub>(this->links->tsAddress(), context->loggerFactory);
        /**
         *
        this->schedule<FlightControl>(1.0f, [](TickingContext *ticking, FlightControl *this_) {
            // this_->processGsCommands(ticking);
        }); //
         */
        this->schedule<FlightControl>(1.0f, [](TickingContext *ticking, FlightControl *this_) {
            this_->processTsCommands(ticking);
        }); //
    }

    void processGsCommands(TickingContext *ticking) {
        // Or use sync lock to access component fields.

        Bridge<FcSkeleton> *gsBridge = 0;
        ticking->ret = this->bridgeKeeperGs->get(gsBridge, FlightControl::createSkeleton, GsStub::create, this, ticking->rst);
        if (ticking->ret < 0) {
            return;
        }

        // connected already.
        GsApi *gsApi = gsBridge->stub<GsApi>();
        ticking->ret = gsApi->ping("hello gs, this is fc.", ticking->rst);
        if (ticking->ret < 0) {
            return;
        }
        if (ticking->ret < 0) {
            int ret = gsApi->log("cannot send sensors data, error to get data from sensor," << ticking->rst.errorMessage, ticking->rst);
            if (ret < 0) {
                ticking->rst << "failed to log message to gs.";
            }
        } else {
            SensorsData sd;
            ticking->rst << "failed to send sensor dat to gs.";
        }
    }
    void processTsCommands(TickingContext *ticking) {
        log(">>processTsCommands");

        log(">>processTsCommands");
    }

    void doProcessTsCommands(TickingContext *ticking) {
        Bridge<FcSkeleton> *tsBridge = 0;
        int ret = bridgeKeeperTs->get(tsBridge, FlightControl::createSkeleton, TsStub::create, this, ticking->rst);
        if (ret < 0) {
            ticking->ret = ret;
            ticking->rst << "failed to establish ts bridge.";
            return;
        }

        // connected already.
        TsApi *tsApi = tsBridge->stub<TsApi>();
    }

    virtual void populate(StagingContext *context) override {
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

    int getTotalServos() {
        return this->totalServos_;
    }
};

} // namespace a8::fc