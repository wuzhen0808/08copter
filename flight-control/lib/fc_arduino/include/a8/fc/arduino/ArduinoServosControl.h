#pragma once
#include "a8/fc.h"

using namespace a8::fc;
namespace a8::fc::arduino_ {

class ArduinoServosControl : public ServosControl {

private:
public:
    ArduinoServosControl(int totalServos, LoggerFactory *logFac);
    ~ArduinoServosControl();
    void setThrottleNorm(int servoId, float velocity) override;
};

} // namespace a8::fc::arduino_
