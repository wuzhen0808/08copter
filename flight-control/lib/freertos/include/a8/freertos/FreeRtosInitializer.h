#pragma once
#include "a8/core/Timer.h"
#include <FreeRTOS.h>
#include <timers.h>
namespace a8::freertos {
using namespace a8::core;

class FreeRtosInitializer {
public:
    FreeRtosInitializer();
    ~FreeRtosInitializer();
    virtual void initialize();
};

} // namespace a8::freertos
