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
        defaultLogger = new WriterLogger("default", new EmptyWriter(), sys);
    }
    virtual Logger *getLogger(const String &name) override {
        return defaultLogger;
    }
};
} // namespace a8::hal::arduino