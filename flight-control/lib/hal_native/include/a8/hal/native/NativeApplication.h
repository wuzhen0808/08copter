#include "a8/hal/Hal.h"
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
#include "a8/hal/native/NativeFileReader.h"
#include "a8/hal/native/NativeLoggerFactory.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Application.h"
#include "a8/util/Finally.h"
#include "a8/util/Result.h"
#include "a8/util/Util.h"
#include "a8/util/WrapperComponent.h"

using namespace a8::util;
using namespace a8::freertos;
using namespace a8::core;
using namespace a8::hal::native;
using namespace a8::hal;
namespace a8::hal::native {

class NativeApplication : public Application {
private:
    Sockets *sFac;
    JSBSimIO *jio;
    Copter *copter;
    String propertiesFile;

protected:
    virtual void boot(Context *context) override {
        Application::boot(context);
    }

    virtual void populate(Context *context) override {
        Application::populate(context);
        sFac = this->addChild<WrapperComponent<NativeSockets>>(context, new WrapperComponent<NativeSockets>(new NativeSockets()))->wrapped;
        if (context->isStop()) {
            return;
        }
        jio = this->addChild<JSBSimIO>(context, new JSBSimIO(sFac));
        if (context->isStop()) {
            return;
        }
        copter = this->addChild<NativeCopter>(context, new NativeCopter(jio));
    }

    virtual void setup(Context *context) override {
        Application::setup(context);
    }

    virtual void start(Context *context) override {
        Application::start(context);
    }

    virtual Scheduler *createScheduler() override {
        return new FreeRtosScheduler();
    }

    virtual LoggerFactory *createLoggerFactory() override {
        return new NativeLoggerFactory();
    }

public:
    static String resolveConfFile(Properties *pts) {

        String fpath = pts->getString("a8.properties", "");
        if (fpath != "") {
            return fpath;
        }
        const char *file = std::getenv("A8_PROPERTIES");
        if (file != 0) {
            return String::string(file);
        }
        return "";
    }
    static void loadDefaultProperties(Properties *properties) {
        properties->set(P_fcs_servo_idx_ar, 0);
        properties->set(P_fcs_servo_idx_fl, 1);
        properties->set(P_fcs_servo_idx_al, 2);
        properties->set(P_fcs_servo_idx_fr, 3);
        properties->set(P_fcs_servo_fr_clockwise, true);
        properties->set(P_fcs_att_tick_rate, 1000); // HZ
    }
    static int start(int argc, char **argv) {
        Properties *pts = new Properties();
        // build int properties
        loadDefaultProperties(pts);

        // command line arguments
        Buffer<String> buf;
        Buffer<String> &args = String::strings(argc, argv, buf);
        for (int i = 0; i < args.getLength(); i++) {
            String str = args.get(i);
            if (str.getLength() > 10000) {
                Util::bug();
            }
        }
        pts->setLines(args);

        // configuration file
        String fpath = resolveConfFile(pts);
        S->out->println(String::format("a8 properties file path:%s", fpath.getText()));

        if (fpath != 0) {
            NativeFileReader fr(fpath);
            Properties *pts2 = new Properties();
            pts2->load(fr);
            pts2->mergeFrom(pts);
            pts = pts2;
        }
        //pts = new Properties();
        Application *app = new NativeApplication();
        app->start(pts);
        delete pts;

        return 0;  
    }

    NativeApplication() : Application("app") {
    }
};
} // namespace a8::hal::native
