#include "Copter.h"
#include "Thread.h"
#include "AttitudeControl.h"
#include "Runnable.h"
#define TOTAL_COMPONENTS 10

Copter::Copter() {
}

Copter::~Copter() {
}

void Copter::active() {
    Serial.begin(9600);
    while (!Serial) {
    }
    this->log("active copter ....");

    AttitudeControl *attitudeControl = new AttitudeControl(this);
    
    Thread* thread = Thread::start(attitudeControl);

    Thread::startScheduler();
    this->log("done of active copter.");
}
void Copter::destroy() {
    
}
int Copter::getServoCount() {
    return (*this).servoCount;
}
int Copter::getServoAttachPin(int servoId) {
    return (*this).servoAttachPins[servoId];
}
void Copter::log(String message){
    Serial.println(message);
}
