#include "a8/fc/arduino/Mpu9250AttitudeSensor.h" /
#include <MPU9250.h>

namespace a8::fc::arduino {

Mpu9250AttitudeSensor::Mpu9250AttitudeSensor(LoggerFactory *logFac) : AttitudeSensor(), FlyWeight(logFac) {
    mpu = new MPU9250();
    logger->info(">>setup MPU9250");
    Lang::cast<MPU9250 *>(mpu)->setup(0x68);
    logger->info("<<setup MPU9250");
}

Mpu9250AttitudeSensor::~Mpu9250AttitudeSensor() {
    Lang::free<MPU9250>(mpu);
}
int Mpu9250AttitudeSensor::isReady(Result &rst) {
    logger->info("Mpu9250AttitudeSensor::isReady()");
    return 1;
}
int Mpu9250AttitudeSensor::getAngVel(Vector3f &angVel, Result &rst) {
    MPU9250 *m = Lang::cast<MPU9250 *>(mpu);
    m->update();
    Vector3f v;
    v.x = m->getRoll();
    v.y = m->getPitch();
    v.z = m->getYaw();
    angVel = v;
    return 1;
}
int Mpu9250AttitudeSensor::getAccVel(Vector3f &accVel, Result &rst) {
    Vector3f v;
    return 1;
}
int Mpu9250AttitudeSensor::getAltitude(double &altitude, Result &rst) {
    altitude = 1;
    return 1;
}
int Mpu9250AttitudeSensor::getLongitude(double &longitude, Result &rst) {
    longitude = 1;
    return 1;
}

} // namespace a8::fc::arduino