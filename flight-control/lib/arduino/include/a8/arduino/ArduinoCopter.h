#pragma once
#include "a8/core/Copter.h"

using a8::core::Copter;
using a8::core::Scheduler;

namespace a8::arduino {

class ArduinoCopter : public Copter {
public:
    ArduinoCopter();
    ~ArduinoCopter();
    virtual void start();
    virtual void stop();
    virtual int getServoAttachPin(int servoId);
    virtual Scheduler* getScheduler();

private:
    int *servoAttachPins;
    Scheduler *scheduler;
};

} // namespace a8::arduino
