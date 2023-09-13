#pragma once
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::util {

class Output {
public:
    virtual void print(String msg) = 0;
    void print(int ivalue);
    void println(String msg);
};
} // namespace a8::util