
#pragma once

#include "a8/hal/Hal.hpp"
#include "a8/util/Output.h"
#include <iostream>

using namespace std;
using namespace a8::util;
using namespace a8::hal;
namespace a8::native {

class NativeSystem : public System {
public:
    NativeSystem();
    virtual long millis();
    virtual void memcpy(void * to, const void * from, int size);
};

} // namespace a8::native
