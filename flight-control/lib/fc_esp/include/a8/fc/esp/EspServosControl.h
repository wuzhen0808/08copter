#pragma once
#include "a8/fc.h"

namespace a8::fc::esp {
using namespace a8::fc;

class EspServosControl : public ServosControl {

private:
public:
    EspServosControl(int totalServos, LoggerFactory *logFac);
    ~EspServosControl();
    void setThrottleNorm(int servoId, float velocity) override;
};

} // namespace a8::fc::arduino_
