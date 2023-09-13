#include "a8/mpu9250/Mpu9250AttitudeSensor.h" /
#include <MPU9250.h>
namespace a8 {
namespace mpu9250 {

Mpu9250AttitudeSensor::Mpu9250AttitudeSensor(Copter *copter) : AttitudeSensor(copter) {
    mpu.setup(0x68);
}

Mpu9250AttitudeSensor::~Mpu9250AttitudeSensor() {
}
void Mpu9250AttitudeSensor::update() {
    //log("Mpu9250AttitudeSensor::update");
    mpu.update();
}

float Mpu9250AttitudeSensor::getRoll() {
    return mpu.getRoll();
}

float Mpu9250AttitudeSensor::getPitch() {
    return mpu.getPitch();
}

float Mpu9250AttitudeSensor::getYaw() {
    return mpu.getYaw();
}
} // namespace mpu9250

} // namespace a8