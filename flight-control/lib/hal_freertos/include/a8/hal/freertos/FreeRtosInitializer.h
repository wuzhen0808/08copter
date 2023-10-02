#pragma once
#include "a8/util/Timer.h"
#include <FreeRTOS.h>
#include <timers.h>

using namespace a8::util;
namespace a8::hal::freertos {

class FreeRtosInitializer {
public:
    FreeRtosInitializer();
    ~FreeRtosInitializer();
    virtual void initialize();
};

} // namespace a8::hal::freertos
