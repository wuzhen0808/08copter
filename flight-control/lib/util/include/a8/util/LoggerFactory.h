#pragma once

#include "a8/util/Logger.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {

class LoggerFactory {
protected:
public:
    LoggerFactory() {}
    ~LoggerFactory() {}
    virtual Logger *getLogger(const String &name) = 0;
};

} // namespace a8::util
