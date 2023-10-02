#pragma once
#include "a8/hal/freertos/FreeRtosInitializer.h"
namespace a8::hal::native {

using a8::hal::freertos::FreeRtosInitializer;


class NativeFreeRtosInitializer : public FreeRtosInitializer {
public:
    NativeFreeRtosInitializer();
    ~NativeFreeRtosInitializer();
    virtual void initialize();
};

} // namespace a8::hal::freertos
