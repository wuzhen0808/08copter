#include "a8/native/NativeFreeRtosInitializer.h"
#include "port.h"//see lib/FreeRTOS-MSVC-MingW/include/port.h
#include "main_from_demo.h"

namespace a8::native {
NativeFreeRtosInitializer::NativeFreeRtosInitializer() {
}

NativeFreeRtosInitializer::~NativeFreeRtosInitializer() {
}

void NativeFreeRtosInitializer::initialize() {
    FreeRtosInitializer::initialize();
    
    //main_from_demo();
}

} // namespace a8::freertos