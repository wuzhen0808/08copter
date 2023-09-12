#include "a8/arduino/ArduinoCopter.h"
#include <Arduino.h>

using namespace a8::core;
using namespace a8::simulator;
using namespace a8::arduino;
namespace a8 {
namespace arduino {

ArduinoCopter::ArduinoCopter():Copter(4, new ArduinoScheduler()) {

}

ArduinoCopter::start(){
   
    Serial.begin(9600);
    while (!Serial) {
    }
    
    Copter *copter = new Copter(scheduler);
    ServosControl *servosControl = new ArduinoServosControl(copter);
    servosControl->attachAll();
    servosControl->setVelocity(0, 1.0f);
    /*
     */
    AttitudeSensor *attitudeSensor = new SimulatorAttitudeSensor(copter);
    AttitudeControl *attitudeControl = new AttitudeControl(copter, servosControl, attitudeSensor);
    copter->log("active copter ....");
    Timer *attitudeTimer = scheduler->scheduleTimer(attitudeControl, 1000);
    scheduler->startSchedule();
    // scheduler->schedule(attitudeControl);
    // attitudeControl->run();
    // copter->log("done of active copter.");
}

} // namespace arduino
} // namespace a8