
#pragma once

#include "a8/hal.h"
#include "a8/hal/native/NativeOutput.h"
#include "a8/util.h"
#include <time.h>
using namespace std;
using namespace a8::util;
using namespace a8::hal;
using namespace a8::util::sys;
namespace a8::hal::native {

class NativeSystem : public System {

public:
    NativeSystem();
    void sleep(long ms) override;
    long millis() override;
};

} // namespace a8::hal::native
