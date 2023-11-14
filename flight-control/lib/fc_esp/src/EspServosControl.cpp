#include "a8/fc/esp/EspServosControl.h"

namespace a8::fc::esp {

EspServosControl::EspServosControl(int totalServos, LoggerFactory *logFac) : ServosControl(totalServos, logFac) {
}

void EspServosControl::setThrottleNorm(int servoId, float velocity) {
    //logger->info("ArduinoServosControl::setThrottleNorm");
}

EspServosControl::~EspServosControl() {
}

} // namespace a8::fc::arduino_