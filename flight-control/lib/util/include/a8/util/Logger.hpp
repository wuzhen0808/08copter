#pragma once

#include "a8/util/Shared.hpp"
#include "a8/util/String.hpp"

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
