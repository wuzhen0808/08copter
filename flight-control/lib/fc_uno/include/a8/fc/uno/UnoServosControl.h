#pragma once
#include "a8/fc.h"

namespace a8::fc::arduino_ {
using namespace a8::fc;

class UnoServosControl : public ServosControl {

private:
public:
    UnoServosControl(int totalServos, LoggerFactory *logFac);
    ~UnoServosControl();
    void setThrottleNorm(int servoId, float velocity) override;
};

} // namespace a8::fc::arduino_
