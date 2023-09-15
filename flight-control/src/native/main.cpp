#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/native/NativeCopter.h"
#include "a8/native/NativeSystem.h"
#include <iostream>

using a8::freertos::FreeRtosScheduler;
using a8::native::NativeCopter;
using a8::native::NativeSystem;
using a8::freertos::FreeRtosInitializer;

System *a8::hal::S = new NativeSystem();

// int main_blinky(int argc, char **argv) {

/**
 * see lib/native/src/main.c
*/


void setup(Scheduler *scheduler){
    scheduler->tmpTimer();
}
int main(int argc, char ** argv) {
     // S->out->println("Hello!");
    //FreeRtosInitializer * rtos = new FreeRtosInitializer();
    //rtos->initialize();
    //S->out->println("RTOS initialized!");
    
    Scheduler *scheduler = new FreeRtosScheduler();
    scheduler->tmpTimer();
    scheduler->tmpTimer();
    Copter *copter = new NativeCopter(scheduler);
    Scheduler * scheduler2 = copter->getScheduler();
    scheduler2->tmpTimer();
    setup(scheduler);
    copter->setup();
    copter->start();
    cout << "Hello,main()";

}


