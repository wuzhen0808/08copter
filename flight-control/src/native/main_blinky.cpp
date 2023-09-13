#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/native/NativeCopter.h"
#include "a8/native/NativeSystem.h"
#include <iostream>

using a8::freertos::FreeRtosScheduler;
using a8::native::NativeCopter;
using a8::native::NativeSystem;

System *a8::hal::S = new NativeSystem();

// int main_blinky(int argc, char **argv) {
extern "C" {

void main_blinky(void) {
    S->out->println("Hello, String");
    Scheduler *scheduler = new FreeRtosScheduler();
    Copter *copter = new NativeCopter(scheduler);
    copter->setup();
    copter->start();
    cout << "Hello,main()";

    copter->setup();
    copter->start();
}
}
