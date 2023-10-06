#pragma once
#include "a8/gs/Dashboard.h"
#include "a8/gs/GsSkeleton.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
namespace a8::gs {

class GroundStation : public Component {

    Dashboard *dashboard;
    Links *network;
    FcApi *fcApi;
    Address *gsAddress;

public:
    GroundStation(int argc, char **argv, Links *network) : Component("gs") {
        this->network = network;
        this->rate = Rate::ForEver;
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
    }

    void setup(StagingContext *context) override {
        Component::setup(context);

        // open dashboard
        this->dashboard = new Dashboard(context->loggerFactory);
        this->gsAddress = network->gsAddress();

        // waiting fcs to connect
        Result rst;
        int ret = this->gsAddress->bind(rst);
        if (ret < 0) {
            return context->stop(rst.errorMessage);
        }

        ret = gsAddress->listen(rst);
        if (ret < 0) {
            return context->stop(rst.errorMessage);
        }
        log("listening connect in on port of gs.");
    }
    static void release() {
    }
    void run(TickingContext *ticking) override {

        log("GS net accepter running...");
        while (true) {
            Bridge *bridge = 0;
            Result rst;
            int ret = gsAddress->accept(
                bridge,
                new GsSkeleton(this->dashboard, this->loggerFactory), GsSkeleton::release, //
                FcStub::create, FcStub::release,                                           //
                rst                                                                        //
            );

            if (ret < 0) {
                log(rst.errorMessage);
                break;
            }
            log("A new GS client connected in.");
        }

        log("Warning: GS main loop exit.");
    }

    void shutdown(StagingContext *context) override {
        Component::shutdown(context);
    }
};

} // namespace a8::gs