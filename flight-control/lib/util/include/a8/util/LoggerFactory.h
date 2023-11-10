#pragma once

#include "a8/util/Logger.h"
#include "a8/util/String.h"

namespace a8::util {
using namespace a8::util;


class LoggerFactory {
protected:
public:
    LoggerFactory() {}
    ~LoggerFactory() {}
    Logger *getLogger() {
        return getLogger("default");
    }

    virtual Logger *getLogger(const String &name) = 0;
};

} // namespace a8::util
