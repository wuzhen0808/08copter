#pragma once
#include "a8/util.h"

using namespace a8::util;

namespace a8::hal::arduino {
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
} // namespace a8::hal::arduino