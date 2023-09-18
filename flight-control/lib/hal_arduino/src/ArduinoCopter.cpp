#include "a8/arduino/ArduinoCopter.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/arduino/ArduinoServosControl.h"
#include "a8/simulator/SimulatorAttitudeSensor.h"
#include <Arduino.h>

using a8::freertos::FreeRtosScheduler;
using a8::arduino::ArduinoServosControl;
using a8::core::AttitudeControl;
using a8::core::AttitudeSensor;
using a8::core::ServosControl;
using a8::core::Thread;
using a8::core::Timer;
using a8::simulator::SimulatorAttitudeSensor;
namespace a8 {
namespace arduino {

ArduinoCopter::ArduinoCopter(Scheduler* scheduler) : Copter(4, scheduler) {
    this->servoAttachPins = new int[4]{3, 5, 9, 11};
    this->scheduler = new FreeRtosScheduler();
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

ServosControl *ArduinoCopter::newServosControl() {
    return new ArduinoServosControl(this);
}

AttitudeSensor *ArduinoCopter::newAttitudeSensor() {
    return new ArduinoAttitudeSensor(this);
}

void ArduinoCopter::start() {
    Copter::start();
}

} // namespace arduino
} // namespace a8