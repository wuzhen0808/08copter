#pragma once
#include "a8/util/Output.h"
#include <iostream>
using namespace std;
namespace a8::hal::native {
using a8::util::Output;
using a8::util::String;

class NativeOutput : public Output {
    virtual void print(const String &msg) override {
        cout << msg.getText();
    }
};

} // namespace a8::hal::native
