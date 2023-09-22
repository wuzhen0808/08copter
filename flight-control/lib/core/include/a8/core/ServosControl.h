#pragma once
#include "a8/core/Component.h"
#include "a8/hal/Servo.h"
#include "a8/util/Result.h"
namespace a8::core {
using a8::hal::Servo;
using a8::util::Result;

class ServosControl : public Component {
protected:
    Servo **servos;
    int totalServos;
    float *lastVelocities;

public:
    ServosControl(int totalServos);
    ~ServosControl();
    virtual Result setup();
    virtual Servo *setupServo(int servoId) = 0;

    void setVelocity(int servoId, float velocity);
    void setVelocities(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4);
};

} // namespace a8::core
