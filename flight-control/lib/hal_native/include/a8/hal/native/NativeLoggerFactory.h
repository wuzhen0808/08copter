#pragma once
#include "a8/hal/native/FileWriter.h"
#include "a8/util/LoggerFactory.h"
#include "a8/util/WriterLogger.h"

using namespace a8::util;

namespace a8::hal::native {
class NativeLoggerFactory : public LoggerFactory {
    Logger *defaultLogger;

public:
    NativeLoggerFactory() {
        defaultLogger = new WriterLogger("default", new FileWriter("D:/tmp/a8.log"));
    }
    virtual Logger *getLogger(const String &name) override {
        return defaultLogger;
    }
};

} // namespace a8::hal::native
