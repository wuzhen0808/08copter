#include "a8/hal/arduino/HardwareI2CWrapper.h"
#include "a8/util.h"
#include <api/HardwareI2C.h>
using namespace arduino;

namespace a8::hal::arduino_ {

HardwareI2CWrapper::HardwareI2CWrapper(HardwareI2C *i2c) {
    this->i2c = i2c;
}
} // namespace a8::hal::arduino_
