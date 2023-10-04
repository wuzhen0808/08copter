#pragma once
#include "a8/fc/Servo.h"
#include "a8/util/comp.h"
#include "a8/util/Result.h"

using namespace a8::util;
using namespace a8::util::comp;

namespace a8::fc {

class ServosControl : public Component {
protected:
    Servo **servos;
    int totalServos;
    float *lastVelocities;

public:
    ServosControl(int totalServos);
    ~ServosControl();
    void setup(StagingContext *context) override {
        log(String::format(">>setup, totalServos:%i", totalServos));

        this->servos = new Servo *[totalServos]; // is Servo need to be a Component type?
        for (int i = 0; i < (*this).totalServos; i++) {
            this->servos[i] = createServo(i);
        }
        // log("ServosControl::active");
        for (int i = 0; i < this->totalServos; i++) {
            this->servos[i]->setThrottleNorm(-1.0f); // initial velocity
        }
    }

    virtual Servo *createServo(int servoId) = 0;

    void setThrottleNorm(int servoId, float velocity);
    void setThrottleNorm(int id1, float vel1, int id2, float vel2, int id3, float vel3, int id4, float vel4);
};

} // namespace a8::fc
