#pragma once

#include "a8/util/Shared.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {

class Logger {
public:
    Logger(String name);
    ~Logger();
    void info(String msg);

private:
    String name;
};

} // namespace a8::util
