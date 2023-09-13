
#pragma once

#include "a8/core/Copter.h"
#include <iostream>

using namespace std;
using namespace a8::util;
using namespace a8::core;
namespace a8::native {

class NativeCopter : public Copter {
public:
    NativeCopter();
    virtual void start();
    virtual void stop();
    virtual int getServoAttachPin(int servoId) {
        return 0;
    }
};

} // namespace a8::native
