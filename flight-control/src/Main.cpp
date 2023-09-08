#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#include <exception>

#include "Copter.h"
#include "Runnable.h"
#include "Thread.h"

void setup() {

    Copter &copter = *(new Copter());
    copter.active();
}

void loop() {
}
