#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/socket/native/NativeSocket.h"
#include "a8/native/NativeCopter.h"
#include "a8/native/NativeSystem.h"
#include <iostream>
#include <stdio.h>

using a8::freertos::FreeRtosInitializer;
using a8::freertos::FreeRtosScheduler;
using a8::hal::socket::Connection;
using a8::hal::socket::native::NativeConnection;
using a8::native::NativeCopter;
using a8::native::NativeSystem;
System *a8::hal::S = new NativeSystem();

// int main_blinky(int argc, char **argv) {

/**
 * see lib/native/src/main.c
 */

void setup(Scheduler *scheduler) {
    scheduler->tmpTimer();
}

float normalizePwm(int pwm) {
    return (pwm - 1500) / 500.0f;
}

void sendServo(Connection *con) {
    // refer to the SIM_JSBSim.cpp from ArduPilot project.
    float aileron = normalizePwm(1500);  // roll
    float elevator = normalizePwm(1500); // pitch
    float rudder = normalizePwm(1500);   // yaw
    float throttle = normalizePwm(1500); // heave

    String message =
        String::format("set fcs/aileron-cmd-norm %f\n", aileron)     //
        + String::format("set fcs/elevator-cmd-norm %f\n", elevator) //
        + String::format("set fcs/rudder-cmd-norm %f\n", rudder)     //
        + String::format("set fcs/throttle-cmd-norm %f\n", throttle) //
        + String("iterate 1\n")                                      //
        ;
    int sent = con->send(message.getText(), message.getLength());
    S->out->println(String::format("sent:%i", sent));
}
void open(Connection *con) {
    String startup =
        "info\n"
        "resume\n"
        "iterate 1\n"
        "set atmosphere/turb-type 4\n";
    con->send(startup.getText(), startup.getLength());

}
int main(int argc, char **argv) {
    Connection *con = new NativeConnection();
    String host = "localhost";
    int port = 5126;
    con->connect(host, port);
    open(con);
    sendServo(con);
    con->close();
    return 0;
}

int mainx(int argc, char **argv) {

    // S->out->println("Hello!");
    // FreeRtosInitializer * rtos = new FreeRtosInitializer();
    // rtos->initialize();
    // S->out->println("RTOS initialized!");

    Scheduler *scheduler = new FreeRtosScheduler();
    scheduler->tmpTimer();
    scheduler->tmpTimer();
    Copter *copter = new NativeCopter(scheduler);
    Scheduler *scheduler2 = copter->getScheduler();
    scheduler2->tmpTimer();
    setup(scheduler);
    copter->setup();
    copter->start();
    cout << "Hello,main()";
    return 0;
}
