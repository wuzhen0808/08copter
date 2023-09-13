#pragma once
#include "a8/util/Output.h"
#include <iostream>
using namespace std;
using a8::util::String;
namespace a8::native {

class NativeOutput : public Output {
    virtual void print(String msg) {
        cout << msg.getText();
    }
};

} // namespace a8::native
