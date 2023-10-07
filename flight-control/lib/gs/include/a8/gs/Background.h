#pragma once
#include "a8/gs/GsSkeleton.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
namespace a8::gs {

class Background : public FlyWeight {

    Links *network;
    FcApi *fcApi;
    Address *gsAddress;
    bool running;

public:
    Background(Links *network, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->network = network;
        this->gsAddress = network->gsAddress();
    }

    int open(Result &rst) {
        // waiting fcs to connect
        int ret = this->gsAddress->bind(rst);
        if (ret < 0) {
            return ret;
        }
        ret = gsAddress->listen(rst);
        if (ret < 0) {
            return ret;
        }
        log("listening connect in on port of gs.");
        return ret;
    }
    void stop() {
        this->running = false;
    }

    void run() {
        running = true;
        log("GS net accepter running...");
        while (running) {
            Bridge *bridge = 0;
            Result rst;
            // TODO timeout for accept.
            int ret = gsAddress->accept(
                bridge,
                new GsSkeleton(this->loggerFactory), GsSkeleton::release, //
                FcStub::create, FcStub::release,                          //
                rst                                                       //
            );

            if (ret < 0) {
                log(rst.errorMessage);
                break;
            }
            log("A new GS client connected in.");
        }

        log(String() << "Warning: GS main loop exit,running:" << running);
        this->running = false;
    }
};

} // namespace a8::gs