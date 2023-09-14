#pragma once
#include "a8/freertos/FreeRtosInitializer.h"
namespace a8::native {

using a8::freertos::FreeRtosInitializer;


class NativeFreeRtosInitializer : public FreeRtosInitializer {
public:
    NativeFreeRtosInitializer();
    ~NativeFreeRtosInitializer();
    virtual void initialize();
};

} // namespace a8::freertos
