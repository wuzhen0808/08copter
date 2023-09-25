#include "a8/core/Copter.h"
#include "a8/core/defines.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/native/NativeCopter.h"
#include "a8/hal/native/NativeLoggerFactory.h"
#include "a8/hal/native/NativeSocket.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Application.h"
#include "a8/util/Finally.h"
#include "a8/util/Result.h"

using namespace a8::util;
using namespace a8::freertos;
using namespace a8::core;
using namespace a8::hal::native;

namespace a8::hal::native {

class NativeApplication : public Application {
private:
    SocketFactory *sFac;
    JSBSimIO *jio;
    Copter *copter;

protected:
    void boot(Context &context) {

        context.properties.set(P_fcs_servo_idx_ar, 0);
        context.properties.set(P_fcs_servo_idx_fl, 1);
        context.properties.set(P_fcs_servo_idx_al, 2);
        context.properties.set(P_fcs_servo_idx_fr, 3);
        context.properties.set(P_fcs_servo_fr_clockwise, true);
        context.properties.set(P_fcs_att_tick_rate, 100); // HZ
        Application::boot(context);
    }

    void populate(Context &context) override {
        sFac = new NativeSocketFactory();
        jio = new JSBSimIO(sFac);
        copter = new NativeCopter(jio);
        this->addChild(jio, context);

        this->addChild(copter, context);
        Application::populate(context);
    }

    void setup(Context &context) override {
        Application::setup(context);
    }

    void start(Context &context) override {
        Application::start(context);
    }

    virtual Scheduler *createScheduler() override {
        return new FreeRtosScheduler();
    }

    virtual LoggerFactory *createLoggerFactory() override {
        return new NativeLoggerFactory();
    }

public:
    NativeApplication() : Application("app") {
        
    }
};
} // namespace a8::hal::native
