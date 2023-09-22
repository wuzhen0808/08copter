#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/JSBSimIO.h"
#include "a8/hal/native/NativeCopter.h"
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
    void populate(Context *context) override {
        sFac = new NativeSocketFactory();

        jio = new JSBSimIO(sFac);
        copter = new NativeCopter(jio);
        this->add(jio);
        this->add(copter);
        Application::populate(context);
    }

    void setup(Context *context) override {
        Application::setup(context);
    }

    void start(Context *context) override {
        this->components->getLength();

        Application::start(context);
    }

    virtual Scheduler *createScheduler() override {
        return new FreeRtosScheduler();
    }
    virtual void log(String msg) override {
        S->out->println(msg);
    }
};
} // namespace a8::hal::native
