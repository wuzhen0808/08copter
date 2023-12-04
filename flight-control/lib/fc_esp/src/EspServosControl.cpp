#include "a8/fc/esp/EspServos.h"

namespace a8::fc::esp {

EspServosControl::EspServosControl(int totalServos, LoggerFactory *logFac) : Servos(totalServos, logFac) {
}

void EspServosControl::setThrottleNorm(int servoId, float velocity) {
    //logger->info("ArduinoServosControl::setThrottleNorm");
}

EspServosControl::~EspServosControl() {
}

} // namespace a8::fc::arduino_