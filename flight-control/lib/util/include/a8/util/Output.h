#pragma once
#include "a8/util/String.hpp"

using namespace a8::util;

namespace a8::util {

class Output {
public:
    virtual void print(String msg);
    void print(int ivalue);
    void println(String msg);
};
} // namespace a8::util