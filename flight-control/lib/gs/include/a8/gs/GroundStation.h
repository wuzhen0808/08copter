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
    String fcHost;
    int fcPort;
    Links *links;
    FcsApi *fcsApi;

public:
    GroundStation(int argc, char **argv, Links *links) : Component("gs") {
        this->links = links;
        this->fcPort = fcPort;
    }

    void populate(Context *context) override {
        Component::populate(context);
    }

    void setup(Context *context) override {
        Component::setup(context);
        this->fcPort = context->properties->getInt("gs/fcs-port", 0);
        if (this->fcPort == 0) {
            context->stop("gs/fcs-port is not set or set as 0");
            return;
        }
        this->fcHost = context->properties->getString("gs/fcs-host", "127.0.0.1");
        // open dashboard
        this->dashboard = new Dashboard();
        // connect to fcs

        int rst = links->getFcsStub(this->fcsApi);
        if (rst < 0) {
            //error to get fcs stub.
        }
    }

    void run() override {
    }

    void shutdown(Context *context) override {
        Component::shutdown(context);
    }
};

} // namespace a8::gs