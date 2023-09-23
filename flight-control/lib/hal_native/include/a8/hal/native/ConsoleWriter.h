#pragma once
#include "a8/hal/Hal.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

using namespace a8::util;

namespace a8::hal::native {
class ConsoleWriter : public Writer {
private:
public:
    virtual void write(char *buf, int bufLen) override {
        S->out->print(String(buf, bufLen));
    }
};
} // namespace a8::hal::native
