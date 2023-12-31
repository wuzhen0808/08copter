#pragma once
#include "a8/hal/arduino/ArduinoOutput.h"
#include "a8/util.h"
#include "a8/util/sched.h"
namespace a8::hal::arduino_ {
using namespace a8::util;
using namespace a8::util::sched;

class ArduinoLoggerFactory : public LoggerFactory {
    Logger *rootLogger;
    System *sys;
    Scheduler *sch;

public:
    ArduinoLoggerFactory(System *sys, Scheduler *sch, int serialNum) {
        this->sys = sys;
        MultiWriter *mw = new MultiWriter();
        Output *output = createOutput(serialNum);
        OutputWriter *ow = new OutputWriter(output);
        mw->add(ow);
        rootLogger = new SyncLogger("syncRoot", new WriterLogger("root", mw, sys), sch);
    }

    Logger *getRootLogger() override {
        return rootLogger;
    }
};
} // namespace a8::hal::arduino_