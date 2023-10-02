
#pragma once

#include "a8/hal.h"
#include "a8/util/Output.h"
#include <iostream>

using namespace std;
using namespace a8::util;
using namespace a8::hal;
namespace a8::hal::native {

class NativeSystem : public System {
public:
    NativeSystem();
    virtual long millis();    
};

} // namespace a8::native
