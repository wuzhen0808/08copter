#pragma once
#include "a8/hal/native/FileWriter.h"
#include "a8/util/LoggerFactory.h"
#include "a8/util/WriterLogger.h"

namespace a8::hal::native {
using namespace a8::util;

class NativeLoggerFactory : public LoggerFactory {
    Logger *defaultLogger;
    System *sys;

public:
    NativeLoggerFactory(System *sys) {
        this->sys = sys;
        defaultLogger = new WriterLogger("default", new FileWriter("D:/tmp/a8.log"), sys);
    }
    virtual Logger *getLogger(const String &name) override {
        return defaultLogger;
    }
};

} // namespace a8::hal::native
