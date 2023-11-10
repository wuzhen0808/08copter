#pragma once
#include "a8/util/Output.h"
namespace a8::hal::native {
using namespace std;
using a8::util::Output;
using a8::util::String;

class NativeOutput : public Output {
    virtual void print(const String msg) override ;
};

} // namespace a8::hal::native
