#pragma once
#include "a8/util/Output.h"
#include <iostream>
using namespace std;
namespace a8::native {
using a8::util::String;

class NativeOutput : public Output {
    virtual void print(String msg) {
        cout << msg.getText();
    }
};

} // namespace a8::native
