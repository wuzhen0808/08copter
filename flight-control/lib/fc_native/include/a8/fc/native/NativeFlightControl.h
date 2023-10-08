
#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/native/FcSkeleton.h"
#include "a8/fc/native/JSBSimIO.h"
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
    Sockets *sockets;
    String propertiesFile;
    String host;
    int port;
    int argc;
    char **argv;
    //
    Links *links;
    FcSkeleton *skeleton;
    Bridge *gsBridge;
    SimInSkeleton *sis;

    GsApi* gsApi;    
    SimOutStub* soStub;

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
    virtual ServosControl *createServosControl(StagingContext *context) override {
        ServosControl *servos = new NativeServosControl(totalServos_, soStub);
        return servos;
    }
    virtual AttitudeSensor *createAttitudeSensor(StagingContext *context) override {
        NativeAttitudeSensor *sensor = new NativeAttitudeSensor(this->sis);
        return sensor;
    }

public:
    NativeFlightControl(int argc, char **argv, Links *links, Sockets *sockets) : FlightControl("fcs", 4) {
        this->sockets = sockets;
        this->links = links;
        this->argc = argc;
        this->argv = argv;
        this->rate = Rate::ForEver;
    }
    ~NativeFlightControl() {}

    virtual void boot(StagingContext *context) override {
        resolveProperties(*context->properties);
        FlightControl::boot(context);
    }
    void heatBeat() {
        gsApi->ping("hello, gs,this is fc.");
    }
    virtual void populate(StagingContext *context) override {
        this->addChild(context, new WrapperComponent<Sockets>(sockets));
        skeleton = new FcSkeleton(context->loggerFactory);

        Result rst;
        int ret = this->links->gsAddress()->connect(this->gsBridge, skeleton, FcSkeleton::release, rst);
        if (ret < 0) {
            context->stop(rst);
            return;
        }
        log("successfully connect to gsApi");

        gsApi = new GsStub(this->gsBridge->getChannel());

        context->scheduler->scheduleTimer([](void *this_) {
            static_cast<NativeFlightControl *>(this_)->heatBeat();
        },
                                          this, 1.0f);
        // TODO enable mock jio for easy validation.
        // jio = new JSBSimIO(sockets); // TODO use network util.
        // this->addChild(context, jio);
        // if (context->isStop()) {
        //     return;
        // }
        //
        sis = new SimInSkeleton(context->loggerFactory);
        Bridge *sisB = 0;
        ret = this->links->simInAddress()->accept(sisB, sis, SimInSkeleton::release, rst);
        if (ret < 0) {
            context->stop(rst);
        }
        //
        Bridge *soBridge = 0;
        ret = this->links->simOutAddress()->connect(soBridge, new SimOutSkeleton(loggerFactory), SimOutSkeleton::release, rst);
        if (ret < 0) {
            return context->stop(rst);
        }
        this->soStub = new SimOutStub(soBridge->getChannel());

        FlightControl::populate(context);
    }

    void run(TickingContext *context) override {
    }

    void setup(StagingContext *context) override {
        FlightControl::setup(context);
    }

    void start(StagingContext *context) override {
        FlightControl::start(context);
    }
};

} // namespace a8::fc::native
