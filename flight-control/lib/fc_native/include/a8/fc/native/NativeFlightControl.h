
#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/native/NativeAttitudeSensor.h"
#include "a8/fc/native/NativeServosControl.h"
#include "a8/fc/native/SimInSkeleton.h"
#include "a8/fc/native/SimOutSkeleton.h"
#include "a8/fc/native/SimOutStub.h"
#include "a8/fc/native/defines.h"
#include "a8/hal.h"
#include "a8/hal/freertos.h"
#include "a8/hal/native.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"

using namespace a8::fc;
using namespace a8::hal::native;
using namespace a8::link;
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
using namespace std;

namespace a8::fc::native {

class NativeFlightControl : public FlightControl {
private:
protected:
    static void run_(TickingContext *tc, NativeFlightControl *this_) {
        this_->run(tc);
    }

public:
    
    NativeFlightControl(int argc, char **argv, Links *links) : FlightControl("fcs", argc, argv, links, 4) {
        this->schedule(Rate::RUN, run_);
    }

    ~NativeFlightControl() {}

    virtual void populate(StagingContext *context) override {

        // TODO enable mock jio for easy validation.
        // jio = new JSBSimIO(sockets); // TODO use network util.
        // this->addChild(context, jio);
        // if (context->isStop()) {
        //     return;
        // }
        //
        Result rst;
        Address *simAddress = this->links->simInAddress();
        int ret = simAddress->bind(rst);
        if (ret < 0) {
            return context->stop(rst);
        }
        ret = simAddress->listen(rst);
        if (ret < 0) {
            return context->stop(rst);
        }

        servosControl_ = new NativeServosControl(totalServos_, context->loggerFactory);
        attitudeSensor_ = new NativeAttitudeSensor();
        FlightControl::populate(context);
    }

    void run(TickingContext *context) {

        Bridge<SimInSkeleton> *simInBridge = 0;
        Bridge<SimOutSkeleton> *simOutBridge = 0;
        while (true) {
            int ret = -1;
            Result rst;

            // waiting the simulator connect in.
            Bridge<SimInSkeleton> *lastSimInBridge = simInBridge;
            SimInSkeleton *skeleton2 = new SimInSkeleton(context->getStaging()->loggerFactory);
            ret = this->links->simInAddress()->accept<SimInSkeleton>(simInBridge, skeleton2, rst);
            if (ret < 0) {
                delete skeleton2;
                break;
            }
            NativeAttitudeSensor *sensor = Lang::cast<NativeAttitudeSensor *>(this->attitudeSensor_);
            sensor->setSkeleton(skeleton2);
            if (lastSimInBridge != 0) {
                lastSimInBridge->close();
                delete lastSimInBridge;
            }

            // connect to the simulator.
            Bridge<SimOutSkeleton> *lastSimOutBridge = simOutBridge;
            ret = this->links->simOutAddress()->connect<SimOutSkeleton>(simOutBridge, new SimOutSkeleton(loggerFactory), rst);
            if (ret < 0) {
                break;
            }
            simOutBridge->createStub<SimOutStub>(SimOutStub::create, SimOutStub::release);
            SimOutStub *stub = simOutBridge->stub<SimOutStub>();
            Lang::cast<NativeServosControl *>(servosControl_)->setStub(stub);

            if (lastSimOutBridge != 0) {
                lastSimOutBridge->close();
                delete lastSimInBridge;
            }
        }
    }

    void setup(StagingContext *context) override {
        FlightControl::setup(context);
    }

    void start(StagingContext *context) override {
        FlightControl::start(context);
    }
};

} // namespace a8::fc::native
