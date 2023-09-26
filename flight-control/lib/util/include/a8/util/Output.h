#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;

class Output {
public:
    virtual void print(const String &msg) = 0;
    void println(const String &msg) {
        this->print(msg);
        this->print("\n");
    }
};
} // namespace a8::util