#include "a8/fc/arduino/ArduinoServosControl.h"

namespace a8::fc::arduino {

ArduinoServosControl::ArduinoServosControl(int totalServos, LoggerFactory *logFac) : ServosControl(totalServos, logFac) {
}

void ArduinoServosControl::setThrottleNorm(int servoId, float velocity) {
}

ArduinoServosControl::~ArduinoServosControl() {
}

} // namespace a8::fc::arduino