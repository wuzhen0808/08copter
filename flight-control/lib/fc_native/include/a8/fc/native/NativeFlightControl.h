
#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/native/FcSkeleton.h"
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

#include <iostream>

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
    String propertiesFile;
    String host;
    int port;
    int argc;
    char **argv;
    //
    Links *links;

    String resolveConfFile(Properties &pts) {

        String fpath = pts.getString(a8_properties, "");
        if (fpath != "") {
            return fpath;
        }
        const char *file = std::getenv(A8_PROPERTIES);
        if (file != 0) {
            return String::string(file);
        }
        return "";
    }

    void loadDefaultProperties(Properties &properties) {
        properties.set(P_fcs_servo_idx_ar, 0);
        properties.set(P_fcs_servo_idx_fl, 1);
        properties.set(P_fcs_servo_idx_al, 2);
        properties.set(P_fcs_servo_idx_fr, 3);
        properties.set(P_fcs_servo_fr_clockwise, true);
        properties.set(P_fcs_att_tick_rate, 1000); // HZ
    }

    void resolveProperties(Properties &pts) {

        // build int properties
        loadDefaultProperties(pts);

        // command line arguments
        Buffer<String> buf;
        Buffer<String> args = StringUtil::strings(argc, argv);
        for (int i = 0; i < args.length(); i++) {
            String str = args.get(i);
            if (str.length() > 10000) {
                Lang::bug();
            }
        }
        pts.setLines(args);

        // configuration file
        String fpath = resolveConfFile(pts);
        S->out->println(String::format("a8 properties file path:%s", fpath.text()));

        if (fpath != 0) {
            NativeFileReader fr(fpath);
            Properties *pts2 = new Properties();
            pts2->load(fr);
            pts.mergeFrom(pts2, false);
        }
    }

protected:
public:
    NativeFlightControl(int argc, char **argv, Links *links) : FlightControl("fcs", 4) {
        this->links = links;
        this->argc = argc;
        this->argv = argv;
        this->rates.append(Rate::RUN)->append(1.0f); // calling run() in a thread, calling tick in 1Hz.
    }
    ~NativeFlightControl() {}

    virtual void boot(StagingContext *context) override {
        resolveProperties(*context->properties);
        FlightControl::boot(context);
    }

    int connectToGs(Scheduler *scheduler, Bridge *&bridgeRef, Result &rst) {
        FcSkeleton *skeleton = new FcSkeleton(loggerFactory);
        int ret = this->links->gsAddress()->connect(bridgeRef, skeleton, FcSkeleton::release, rst);
        if (ret < 0) {
            delete skeleton;
            return ret;
        }

        bridgeRef->createStub<GsApi>(GsStub::create, GsStub::release);
        // hearBeat

        log("successfully connect to gs");
        return ret;
    }

    void tick(TickingContext *ticking) override {
        // Or use sync lock to access component fields.
        int KEY = 0;
        Bridge *gsBridge = ticking->getVar<Bridge>(KEY);
        if (gsBridge != 0) {
            Result rst;
            int ret = gsBridge->isRunning(rst);
            if (ret < 0) {
                log(rst.errorMessage);
                ticking->setVar(KEY, 0);
                delete gsBridge;
                gsBridge = 0;
            }
        }

        if (gsBridge == 0) {
            Result rst;
            int ret = this->connectToGs(ticking->getStaging()->scheduler, gsBridge, rst);
            if (ret < 0) {
                log(rst);
                // cannot connect , so nothing to do else.
                return;
            }
            ticking->setVar(KEY, gsBridge);
        }
        // connected already.
        GsApi *gsApi = gsBridge->stub<GsApi>();
        gsApi->ping("hello gs, this is fc.");

        SensorsData sd;
        Result rst;
        int ret = attitudeSensor_->getAltitude(sd.altitude, rst);
        if (ret < 0) {
            gsApi->log("cannot send sensors data, error to get data from sensor," << rst.errorMessage);
        } else {
            gsApi->sensors(sd);
        }
    }

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

    void run(TickingContext *context) override {

        Bridge *simInBridge = 0;
        Bridge *simOutBridge = 0;
        while (true) {
            int ret = -1;
            Result rst;

            // waiting the simulator connect in.
            Bridge *lastSimInBridge = simInBridge;
            SimInSkeleton *skeleton2 = new SimInSkeleton(context->getStaging()->loggerFactory);
            ret = this->links->simInAddress()->accept(simInBridge, skeleton2, SimInSkeleton::release, rst);
            if (ret < 0) {
                delete skeleton2;
                break;
            }
            NativeAttitudeSensor *sensor = static_cast<NativeAttitudeSensor *>(this->attitudeSensor_);
            sensor->setSkeleton(skeleton2);
            if (lastSimInBridge != 0) {
                lastSimInBridge->close();
                delete lastSimInBridge;
            }

            // connect to the simulator.
            Bridge *lastSimOutBridge = simOutBridge;
            ret = this->links->simOutAddress()->connect(simOutBridge, new SimOutSkeleton(loggerFactory), SimOutSkeleton::release, rst);
            if (ret < 0) {
                break;
            }
            simOutBridge->createStub<SimOutStub>(SimOutStub::create, SimOutStub::release);
            SimOutStub *stub = lastSimOutBridge->stub<SimOutStub>();
            static_cast<NativeServosControl *>(servosControl_)->setStub(stub);

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
