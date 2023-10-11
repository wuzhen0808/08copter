#pragma once
#include "a8/fc/AttitudeControl.h"
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/FcSkeleton.h"
#include "a8/fc/ServosControl.h"
#include "a8/fc/defines.h"
#include "a8/hal.h"
#include "a8/util.h"
#include "a8/util/net.h"
#include "a8/util/thread.h"

using a8::hal::S;

using namespace a8::util;
using namespace a8::util::thread;
using namespace a8::util::comp;

namespace a8::fc {

class FlightControl : public Component {
    static const int TICKING_KEY_BRIDGE = 0;

protected: // fields
    int totalServos_;
    AttitudeSensor *attitudeSensor_;
    ServosControl *servosControl_;
    AttitudeControl *attitudeControl_;
    String propertiesFile;
    String host;
    int port;
    int argc;
    char **argv;
    //
    Links *links;
    BridgeKeeper<FcSkeleton, GsStub> *bridgeKeeper;

protected: // functions
    static FcSkeleton *createSkeleton(FlightControl *this_) {
        return new FcSkeleton(this_->loggerFactory);
    }

    void init(int argc, char **argv, Links *links) {
        this->links = links;
        this->argc = argc;
        this->argv = argv;
        this->rates.append(Rate::RUN)->append(1.0f); // calling run() in a thread, calling tick in 1Hz.
        this->bridgeKeeper = new BridgeKeeper<FcSkeleton, GsStub>(this->links->gsAddress());
    }

    String resolveConfFile(Properties &pts) {

        String fpath = pts.getString(a8_properties, "");
        if (fpath != "") {
            return fpath;
        }
        String file2 = S->getEnv(A8_PROPERTIES);
        return file2;
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
            Reader *fr;
            int ret = S->openFile(fpath, fr);
            Properties *pts2 = new Properties();
            pts2->load(*fr);
            pts.mergeFrom(pts2, false);
        }
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
        resolveProperties(*context->properties);
        Component::boot(context);
    }

    void tick(TickingContext *ticking) override {
        // Or use sync lock to access component fields.
        Result rst;
        Bridge<FcSkeleton> *gsBridge = 0;
        int ret = bridgeKeeper->get(gsBridge, FlightControl::createSkeleton, GsStub::create, this, rst);
        if (ret < 0) {
            return;
        }

        // connected already.
        GsApi *gsApi = gsBridge->stub<GsApi>();
        gsApi->ping("hello gs, this is fc.");

        SensorsData sd;
        ret = attitudeSensor_->getAltitude(sd.altitude, rst);
        if (ret < 0) {
            gsApi->log("cannot send sensors data, error to get data from sensor," << rst.errorMessage);
        } else {
            gsApi->sensors(sd);
        }
    }
    virtual void populate(StagingContext *context) override {
        Component::populate(context);
        attitudeControl_ = new AttitudeControl(servosControl_, attitudeSensor_);
        this->addChild(context, attitudeControl_);
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