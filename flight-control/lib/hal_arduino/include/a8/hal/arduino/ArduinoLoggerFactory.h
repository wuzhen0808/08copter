#pragma once
#include "a8/util.h"

namespace a8::hal::arduino_ {
using namespace a8::util;

class ArduinoLoggerFactory : public LoggerFactory {
    Logger *defaultLogger;
    System *sys;

public:
    ArduinoLoggerFactory(System *sys) {
        this->sys = sys;
        MultiWriter* mw = new MultiWriter();
        OutputWriter *ow = new OutputWriter(sys->out);
        mw->add(ow);

        defaultLogger = new WriterLogger("default", mw, sys);
    }
    virtual Logger *getLogger(const String &name) override {
        return defaultLogger;
    }
};
} // namespace a8::hal::arduino_