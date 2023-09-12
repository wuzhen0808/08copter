#pragma once
#include "a8/core/Copter.hpp"

using namespace std;
using namespace a8::util;
namespace a8::arduino {

class ArduinoCopter : public Copter {
public:
    ArduinoCopter(int servoCount, Scheduler *scheduler);
    virtual void start();
    virtual void stop();
    virtual int getServoAttachPin(int servoId) {
        return servoAttachPins[servoId];
    }
    
private:
    int[] servoAttachPins = new int[] { 3, 5, 9, 11 }
}

} // namespace a8::arduino
