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
    Network *network;
    FcApi *fcApi;
    Address *gsAddress;

public:
    GroundStation(int argc, char **argv, Network *network) : Component("gs") {
        this->network = network;
        this->rate = Rate::ForEver;
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
    }

    void setup(StagingContext *context) override {
        Component::setup(context);

        // open dashboard
        this->dashboard = new Dashboard();
        this->addChild(context, this->dashboard);
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

    void run(TickingContext *ticking) override {
        log("GS net accepter running...");
        while (true) {
            Channel<GsApi, FcApi> *channel = 0;

            GsApi *skeleton = new GsSkeleton(this->dashboard, this->loggerFactory);
            Result rst;
            int ret = gsAddress->accept<GsApi, FcApi>(channel, skeleton, FcStub::create, FcStub::release, rst);
            if (ret < 0) {
                delete skeleton;
                log(rst.errorMessage);
                break;
            }
            log("A new GS client connected in, we treat it as a FcStub.");

            delete channel;
        }
        log("Warning: GS net accepter exited.");
    }

    void shutdown(StagingContext *context) override {
        Component::shutdown(context);
    }
};

} // namespace a8::gs