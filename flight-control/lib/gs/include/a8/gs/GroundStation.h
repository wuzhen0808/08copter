#pragma once
#include "a8/gs/Dashboard.h"
#include "a8/gs/GsNetImp.h"
#include "a8/link.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
namespace a8::gs {

class GroundStation : public Component {

    Dashboard *dashboard;
    Links *links;
    FcApi *fcApi;

public:
    GroundStation(int argc, char **argv, Links *links) : Component("gs") {
        this->links = links;
    }

    void populate(Context *context) override {
        Component::populate(context);
    }

    void setup(Context *context) override {
        Component::setup(context);

        // open dashboard
        this->dashboard = new Dashboard();
        // waiting fcs to connect

        int ret = links->bindGs(context->message());
        if (ret < 0) {
            return context->stop();
        }

        GsNetImp *gsNet = new GsNetImp(this->dashboard);
        ret = links->listen(gsNet, context->message());
        if (ret < 0) {
            context->stop();
            return;
        }
        log("listening connect in on port of gs.");
        // or connect to fcs
        String errorMessage;
        int rst = links->getStub(this->fcApi, errorMessage);
        if (rst < 0) {
            // context->stop(errorMessage);
            // ignore the error.
        }
    }

    void run() override {
    }

    void shutdown(Context *context) override {
        Component::shutdown(context);
    }
};

} // namespace a8::gs