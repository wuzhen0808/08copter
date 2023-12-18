#pragma once
#include "a8/hal/esp/EspLoggerFactory.h"
#include "a8/hal/esp/EspScheduler.h"
#include "a8/hal/esp/EspSystem.h"
#include "a8/util.h"
#include "a8/util/comp.h"
#include "a8/util/sched.h"

#include <Arduino.h>
#include <Wire.h>
namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::sched;
using namespace a8::hal::esp;
using namespace a8::util::comp;
class EspUtil {
public:
    static int setupSerial() {
        Serial.begin(115200);
        while (Serial.available()) {
            Serial.read();
        }

        return 1;
    }
    static int setupWire() {
        bool ok = Wire.begin(2, 1);
        if (!ok) {
            Serial.println("failed to setup wire.");
        } else {
            Serial.println("success of setup wire.");
        }
        return ok ? 1 : -1;
    }

    static int start(String name, Component *comp, Result &res) {
        System *sys = new EspSystem();
        Scheduler *sch = new EspScheduler();
        LoggerFactory *logFac = new EspLoggerFactory(sys, sch);
        StagingContext *context = new StagingContext(sch,    //
                                                     logFac, //
                                                     sys     //
        );
        Application::start(name, context, comp);
        if (context->isStop()) {
            res.errorMessage << context->getMessage();
            return -1;
        }
        return 1;
    }
};
} // namespace a8::hal::esp