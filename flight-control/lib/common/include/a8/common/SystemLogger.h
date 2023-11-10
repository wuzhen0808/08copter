#pragma once
#include "a8/hal.h"
#include "a8/util/Logger.h"

namespace a8::common {
using namespace a8::util;
using namespace a8::hal;

class SimpleLogger : public Logger {

protected:
    virtual void log(int level, const String &msg) override {
        S->out->print(">>");
        S->out->print(msg);
        S->out->print("<<");
    }

public:
    SimpleLogger(String name) : Logger(name) {
    }
};

} // namespace a8::hal