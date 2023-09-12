#include "a8/arduino/ArduinoCopter.h"
#include "a8/arduino/ArduinoScheduler.hpp"
#include "a8/arduino/ArduinoServosControl.hpp"
#include "a8/core/core_all.hpp"
#include "a8/simulator/SimulatorAttitudeSensor.hpp"
#include <Arduino.h>

using a8::core::AttitudeControl;
using a8::core::AttitudeSensor;
using a8::core::ServosControl;
using a8::core::Thread;
using a8::core::Timer;
using a8::arduino::ArduinoScheduler;
using a8::arduino::ArduinoServosControl;
using a8::simulator::SimulatorAttitudeSensor;
namespace a8 {
namespace arduino {

ArduinoCopter::ArduinoCopter() : Copter(4) {
    this->servoAttachPins = new int[4]{3, 5, 9, 11};
    this->scheduler = new ArduinoScheduler();
}

int ArduinoCopter::getServoAttachPin(int idx) {
    return this->servoAttachPins[idx];
}
Scheduler *ArduinoCopter::getScheduler() {
    return scheduler;
}
ArduinoCopter::~ArduinoCopter() {
    delete[] this->servoAttachPins;
}
void ArduinoCopter::stop() {
}

void ArduinoCopter::start() {

    Serial.begin(9600);
    while (!Serial) {
    }

    ServosControl *servosControl = new ArduinoServosControl(this);
    servosControl->attachAll();
    servosControl->setVelocity(0, 1.0f);

    AttitudeSensor *attitudeSensor = new SimulatorAttitudeSensor(this);
    AttitudeControl *attitudeControl = new AttitudeControl(this, servosControl, attitudeSensor);
    this->log("active copter ....");

    Timer *attitudeTimer = scheduler->scheduleTimer(attitudeControl, 1000);
    scheduler->startSchedule();
    // scheduler->schedule(attitudeControl);
    // attitudeControl->run();
    // copter->log("done of active copter.");
}

} // namespace arduino
} // namespace a8