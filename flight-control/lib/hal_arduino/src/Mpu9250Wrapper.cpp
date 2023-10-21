#include "a8/hal/arduino/Mpu9250Wrapper.h"
#include "a8/util.h"
#include <MPU9250.h>
using namespace arduino;

namespace a8::hal::arduino_ {

Mpu9250Wrapper::Mpu9250Wrapper(MPU9250_<HardwareI2C> *mpu) {
    this->mpu = mpu;
}

void Mpu9250Wrapper::setup(int address, HardwareI2C *wire) {
    MPU9250Setting setting;
    mpu->setup(address, setting, *wire);
}

bool Mpu9250Wrapper::update() {
    return mpu->update();
}
float Mpu9250Wrapper::getRoll() {
    return mpu->getRoll();
}
float Mpu9250Wrapper::getPitch() {
    return mpu->getPitch();
}
float Mpu9250Wrapper::getYaw() {
    return mpu->getYaw();
}
} // namespace a8::fc::arduino_
