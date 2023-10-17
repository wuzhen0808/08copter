#include "a8/fc/arduino/Mpu9250AttitudeSensor.h" /
#include <MPU9250.h>

namespace a8::fc::arduino {

Mpu9250AttitudeSensor::Mpu9250AttitudeSensor() : AttitudeSensor() {
    mpu = new MPU9250();
    Lang::cast<MPU9250 *>(mpu)->setup(0x68);
}

Mpu9250AttitudeSensor::~Mpu9250AttitudeSensor() {
    Lang::free<MPU9250>(mpu);
}
int Mpu9250AttitudeSensor::isReady(Result &rst) {
}
int Mpu9250AttitudeSensor::getAngVel(Vector3f &angVel, Result &rst) {
    MPU9250 *m = Lang::cast<MPU9250 *>(mpu);
    m->getRoll();
    m->getPitch();
}
int Mpu9250AttitudeSensor::getAccVel(Vector3f &accVel, Result &rst) {
}
int Mpu9250AttitudeSensor::getAltitude(double &altitude, Result &rst) {
}
int Mpu9250AttitudeSensor::getLongitude(double &longitude, Result &rst) {
}

} // namespace a8::fc::arduino