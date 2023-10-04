
#pragma once

#include "a8/hal.h"
#include "a8/hal/native/NativeOutput.h"
#include "a8/util.h"
#include "windows.h"
#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
using namespace a8::util;
using namespace a8::hal;
using namespace a8::util::sys;
namespace a8::hal::native {

class NativeSystem : public System {

    struct timespec time;

public:
    NativeSystem() {
        out = new NativeOutput();
    }
    void sleep(long ms) override {
        Sleep(ms);
    }
    long millis() override {
        clock_gettime(CLOCK_REALTIME, &time);

        return time.tv_sec * 1000 + time.tv_nsec / 1.0e6;
    }
};

} // namespace a8::hal::native
