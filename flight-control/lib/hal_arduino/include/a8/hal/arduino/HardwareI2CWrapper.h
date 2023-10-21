#pragma once

namespace arduino {
class HardwareI2C;
} // namespace arduino

using namespace arduino;

namespace a8::hal::arduino_ {

class HardwareI2CWrapper {

public:
    HardwareI2C *i2c;
    HardwareI2CWrapper(HardwareI2C *i2c);
    void setup(int address);
};
} // namespace a8::hal::arduino_