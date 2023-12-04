#pragma once
#include "a8/fc.h"

namespace a8::fc::uno {
using namespace a8::fc;

class UnoServosControl : public Servos {

private:
public:
    UnoServosControl(int totalServos, LoggerFactory *logFac);
    ~UnoServosControl();
    void setThrottleNorm(int servoId, float velocity) override;
};

} // namespace a8::fc::arduino_
