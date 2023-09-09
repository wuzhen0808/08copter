#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <exception>

#include <a8/core/AttitudeControl.hpp>
#include "a8/core/Copter.hpp"
#include "a8/rtos/Runnable.hpp"
#include "a8/mpu9250/Mpu9250AttitudeSensor.hpp"
#include <a8/simulator/SimulatorAttitudeSensor.hpp>

void setup() {
    using namespace a8::core;
    using namespace a8::simulator;
    using namespace a8::rtos;
    {
        
    } // namespace a8::rtos;
    
    Serial.begin(9600);
    while (!Serial) {
    }
    Scheduler *scheduler = new Scheduler();
    Copter *copter = new Copter(scheduler);
    ServosControl *servosControl = new ServosControl(copter);
    servosControl->attachAll();
    servosControl->setVelocity(0, 1.0f);
    /*
     */
    AttitudeSensor *attitudeSensor = new SimulatorAttitudeSensor(copter);
    AttitudeControl *attitudeControl = new AttitudeControl(copter, servosControl, attitudeSensor);
    copter->log("active copter ....");
    Timer* attitudeTimer = scheduler->scheduleTimer(attitudeControl, 1000);
    scheduler->startSchedule();
    // scheduler->schedule(attitudeControl);
    //attitudeControl->run();
    //copter->log("done of active copter.");
}

void loop() {
}
