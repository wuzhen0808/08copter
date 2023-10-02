
#pragma once
#include "a8/fcs/FlightControl.h"
#include "a8/fcs/defines.h"
#include "a8/hal/freertos/FreeRtosInitializer.h"
#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/native/NativeAttitudeSensor.h"
#include "a8/hal/native/NativeFileReader.h"
#include "a8/hal/native/NativeGsStub.h"
#include "a8/hal/native/NativeLoggerFactory.h"
#include "a8/hal/native/NativeServosControl.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Finally.h"
#include "a8/util/Result.h"
#include "a8/util/SocketRunnerComponent.h"
#include "a8/util/SocketWriter.h"
#include "a8/util/Sockets.h"
#include "a8/util/Lang.h"
#include "a8/util/WrapperComponent.h"

#include <iostream>

using namespace a8::hal::native;
using namespace std;
using namespace a8::util;
using namespace a8::net;
using namespace a8::fcs;

namespace a8::hal::native {

class NativeFlightControl : public FlightControl {
private:
    Sockets *sockets;
    JSBSimIO *jio;
    String propertiesFile;
    JSBSimIO *jsbSimIo;
    String host;
    int port;
    int argc;
    char **argv;
    GsStub *gs;
    int gsPort;
    String resolveConfFile(Properties &pts) {

        String fpath = pts.getString("a8.properties", "");
        if (fpath != "") {
            return fpath;
        }
        const char *file = std::getenv("A8_PROPERTIES");
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
        ServosControl *servos = new NativeServosControl(totalServos_, jsbSimIo);
        return servos;
    }
    virtual AttitudeSensor *createAttitudeSensor(Context *context) override {
        NativeAttitudeSensor *sensor = new NativeAttitudeSensor(this->jsbSimIo);
        return sensor;
    }

    virtual GsStub *createGsStub(Context *context) override {

        return new NativeGsStub(sockets, gsPort);
    }

public:
    NativeFlightControl(int argc, char **argv, Sockets *sockets) : FlightControl("fcs", 4) {
        this->sockets = sockets;
        this->argc = argc;
        this->argv = argv;
    }
    ~NativeFlightControl() {}

    virtual void boot(Context *context) override {
        resolveProperties(*context->properties);
        FlightControl::boot(context);
    }

    virtual void populate(Context *context) override {
        FlightControl::populate(context);
        this->addChild<WrapperComponent<Sockets>>(context, new WrapperComponent<Sockets>(sockets));
        if (context->isStop()) {
            return;
        }
        jio = this->addChild<JSBSimIO>(context, new JSBSimIO(sockets));
        if (context->isStop()) {
            return;
        }
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

} // namespace a8::hal::native
