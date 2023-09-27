// clang-comment off
#include "a8/hal/native/NativeSockets.h"
// clang-comment on
#include "a8/core/Copter.h"
#include "a8/core/defines.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/native/NativeCopter.h"
#include "a8/hal/native/NativeLoggerFactory.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Application.h"
#include "a8/util/Finally.h"
#include "a8/util/Result.h"
#include "a8/util/WrapperComponent.h"

using namespace a8::util;
using namespace a8::freertos;
using namespace a8::core;
using namespace a8::hal::native;

namespace a8::hal::native {

class NativeApplication : public Application {
private:
    Sockets *sFac;
    JSBSimIO *jio;
    Copter *copter;
    String* configFile;
protected:
    virtual void boot(Context &context) override {

        context.properties.set(P_fcs_servo_idx_ar, 0);
        context.properties.set(P_fcs_servo_idx_fl, 1);
        context.properties.set(P_fcs_servo_idx_al, 2);
        context.properties.set(P_fcs_servo_idx_fr, 3);
        context.properties.set(P_fcs_servo_fr_clockwise, true);
        context.properties.set(P_fcs_att_tick_rate, 1000); // HZ
        //FileReader reader = new FileReader(configFile);
        //Properties::load();
        Application::boot(context);
    }

    virtual void populate(Context &context) override {
        Application::populate(context);
        sFac = this->addChild<WrapperComponent<NativeSockets>>(context, new WrapperComponent<NativeSockets>(new NativeSockets()))->wrapped;
        if (context.isStop()) {
            return;
        }
        jio = this->addChild<JSBSimIO>(context, new JSBSimIO(sFac));
        if (context.isStop()) {
            return;
        }
        copter = this->addChild<NativeCopter>(context, new NativeCopter(jio));
    }

    virtual void setup(Context &context) override {
        Application::setup(context);
    }

    virtual void start(Context &context) override {
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
