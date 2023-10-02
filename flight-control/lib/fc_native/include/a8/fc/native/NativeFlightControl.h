
#pragma once
#include "a8/fc.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/native/JSBSimIO.h"
#include "a8/fc/native/NativeAttitudeSensor.h"
#include "a8/fc/native/NativeServosControl.h"
#include "a8/fc/native/defines.h"
#include "a8/hal.h"
#include "a8/hal/freertos/FreeRtosInitializer.h"
#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/native.h"
#include "a8/link.h"
#include "a8/util/net.h"

#include <iostream>

using namespace a8::fc;
using namespace a8::hal::native;
using namespace a8::link;
using namespace a8::util;
using namespace a8::util::net;
using namespace std;

namespace a8::fc::native {

class NativeFlightControl : public FlightControl {
private:
    Sockets *sockets;
    JSBSimIO *jio;
    String propertiesFile;
    String host;
    int port;
    int argc;
    char **argv;
    //
    GsApi *gsApi;
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
        Buffer<String> &args = String::strings(argc, argv, buf);
        for (int i = 0; i < args.getLength(); i++) {
            String str = args.get(i);
            if (str.getLength() > 10000) {
                Lang::bug();
            }
        }
        pts.setLines(args);

        // configuration file
        String fpath = resolveConfFile(pts);
        S->out->println(String::format("a8 properties file path:%s", fpath.getText()));

        if (fpath != 0) {
            NativeFileReader fr(fpath);
            Properties *pts2 = new Properties();
            pts2->load(fr);
            pts.mergeFrom(pts2, false);
        }
    }

protected:
    virtual ServosControl *createServosControl(Context *context) override {
        ServosControl *servos = new NativeServosControl(totalServos_, jio);
        return servos;
    }
    virtual AttitudeSensor *createAttitudeSensor(Context *context) override {
        NativeAttitudeSensor *sensor = new NativeAttitudeSensor(this->jio);
        return sensor;
    }

public:
    NativeFlightControl(int argc, char **argv, Links *links, Sockets *sockets) : FlightControl("fcs", 4) {
        this->sockets = sockets;
        this->links = links;
        this->argc = argc;
        this->argv = argv;
    }
    ~NativeFlightControl() {}

    virtual void boot(Context *context) override {
        resolveProperties(*context->properties);
        FlightControl::boot(context);
    }

    virtual void populate(Context *context) override {
        this->addChild(context, new WrapperComponent<Sockets>(sockets));

        Result errorMessage;
        int rst = this->links->getStub(this->gsApi, errorMessage);
        if (rst < 0) {
            context->stop(errorMessage);
            return;
        }
        log("successfully connect to gsApi");
        jio = new JSBSimIO(sockets);
        this->addChild(context, jio);
        if (context->isStop()) {
            return;
        }

        FlightControl::populate(context);
    }

    void setup(Context *context) override {
        FlightControl::setup(context);
    }

    void start(Context *context) override {
        FlightControl::start(context);
    }
    void stop() override {
        FlightControl::stop();
    }
};

} // namespace a8::fc::native
