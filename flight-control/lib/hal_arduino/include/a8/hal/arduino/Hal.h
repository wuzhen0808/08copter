#pragma once
#include "a8/hal/arduino/HardwareI2CWrapper.h"
#include "a8/hal/arduino/HardwareSerialWrapper.h"
#include "a8/hal/arduino/Mpu9250Wrapper.h"

namespace a8::hal::arduino_ {

class Hal {
public:
    HardwareSerialWrapper *serial;
    HardwareI2CWrapper *i2c;
    Mpu9250Wrapper * mpu9250;    
};

} // namespace a8::hal::arduino_