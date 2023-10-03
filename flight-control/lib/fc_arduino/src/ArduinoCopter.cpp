#include "a8/fc/arduino/ArduinoCopter.h"
#include "a8/fc/arduino/ArduinoServosControl.h"
#include "a8/hal/freertos.h"

#include <Arduino.h>

using namespace a8::fc;
using namespace a8::util;
using namespace a8::util::thread;
using namespace a8::hal::freertos;
namespace a8::fc::arduino {

ArduinoCopter::ArduinoCopter(Scheduler *scheduler) : Copter(4, scheduler) {
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

} // namespace a8::fc::arduino