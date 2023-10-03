#pragma once
#include "a8/util/comp.h"
#include "a8/gs/Dashboard.h"
#include "a8/gs/GsNetImp.h"
#include "a8/link.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
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
        Result rst ;
        int ret = links->bindGs(rst);
        if (ret < 0) {            
            return context->stop(rst.errorMessage);
        }

        GsNetImp *gsNet = new GsNetImp(this->dashboard);
        Result rst2;
        ret = links->listen(gsNet, rst2);
        if (ret < 0) {
            context->stop(rst2.errorMessage);
            return;
        }
        log("listening connect in on port of gs.");
        // or connect to fcs
        Result rst3;
        ret = links->getStub(this->fcApi, rst3);
        if (ret < 0) {
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