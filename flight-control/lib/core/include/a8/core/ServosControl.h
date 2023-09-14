#ifndef SERVOS_CONTROL__
#define SERVOS_CONTROL__
#include "a8/core/Component.h"
#include "a8/hal/Servo.h"

namespace a8::core {
using a8::hal::Servo;
class ServosControl : public Component {
protected:
    Servo **servos;
    int totalServos;
    int *servoAttachPins;
    float *lastVelocities;

public:
    ServosControl(int totalServos, int *servoAttachPins);
    ~ServosControl();
    virtual Servo *newServo(int servoId) = 0;

    void attachAll();
    void setVelocity(int servoId, float velocity);
    void setVelocities(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4);
};

} // namespace a8::core
#endif