#include "a8/fc/uno/UnoServosControl.h"

namespace a8::fc::uno {

UnoServosControl::UnoServosControl(int totalServos, LoggerFactory *logFac) : ServosControl(totalServos, logFac) {
}

void UnoServosControl::setThrottleNorm(int servoId, float velocity) {
    logger->info("ArduinoServosControl::setThrottleNorm");
}

UnoServosControl::~UnoServosControl() {
}

} // namespace a8::fc::arduino_