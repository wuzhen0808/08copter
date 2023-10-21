#pragma once

namespace arduino {
class HardwareSerial;
class HardwareI2C;
} // namespace arduino

template <typename T>
class MPU9250_;
using namespace arduino;

namespace a8::hal::arduino_ {

class Mpu9250Wrapper {

public:
    MPU9250_<HardwareI2C> *mpu;
    Mpu9250Wrapper(MPU9250_<HardwareI2C> *mpu);
    void setup(int address, HardwareI2C *wire);
    bool update();
    float getRoll();
    float getPitch();
    float getYaw();
};
} // namespace a8::hal::arduino_