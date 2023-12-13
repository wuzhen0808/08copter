#pragma once
#include "a8/hal/esp.h"
#include "a8/util.h"
#include "a8/util/sched.h"
#include <Wire.h>
namespace a8::hal::esp {
using namespace a8::util;
using namespace a8::util::sched;
using namespace a8::hal::esp;

class BaseEspExample : public FlyWeight {
protected:
    System *sys;
    Scheduler *sch;

    int setupSerial() {
        Serial.begin(115200);
        while (Serial.available()) {
            Serial.read();
        }
        log("success setup serial.");
        return 1;
    }
    int setupWire() {
        bool ok = Wire.begin(2, 1);
        if (!ok) {
            Serial.println("failed to setup wire.");
        } else {
            Serial.println("success of setup wire.");
        }
        return ok ? 1 : -1;
    }

public:
    BaseEspExample(String name) {
        sys = new EspSystem();
        sch = new EspScheduler();
        LoggerFactory *logFac = new EspLoggerFactory(sys, sch);
        FlyWeight::init(logFac, name);
    }

    virtual int start(Result &res) = 0;
};
} // namespace a8::hal::esp::example