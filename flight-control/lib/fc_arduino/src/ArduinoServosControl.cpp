#include "a8/fc/arduino/ArduinoServosControl.h"

namespace a8::fc::arduino_ {

ArduinoServosControl::ArduinoServosControl(int totalServos, LoggerFactory *logFac) : ServosControl(totalServos, logFac) {
}

void ArduinoServosControl::setThrottleNorm(int servoId, float velocity) {
    logger->info("ArduinoServosControl::setThrottleNorm");
}

ArduinoServosControl::~ArduinoServosControl() {
}

} // namespace a8::fc::arduino_