#pragma once
#include "a8/util/String.h"

namespace a8::util {

using a8::util::String;

class Output {
public:
    virtual void print(String msg) = 0;
    void print(int ivalue);
    void println(String msg);
};
} // namespace a8::util