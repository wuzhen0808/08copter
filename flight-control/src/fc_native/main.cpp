#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/socket/native/NativeSocket.h"
#include "a8/native/NativeCopter.h"
#include "a8/native/NativeSystem.h"
#include "a8/util/Finally.h"
#include <iostream>
#include <stdio.h>

using a8::freertos::FreeRtosInitializer;
using a8::freertos::FreeRtosScheduler;

using a8::hal::socket::Socket;
using a8::hal::socket::SocketFactory;
using a8::hal::socket::native::NativeSocket;
using a8::native::NativeCopter;
using a8::native::NativeSystem;
using a8::util::String;

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

void sendServo(Socket *con) {
    // refer to the SIM_JSBSim.cpp from ArduPilot project.
    /*
    String sb;
    for (int i = 0; i < 4; i++) {
        float throttleI = normalizePwm(2000);
        String tmp = String::format("set fcs/throttle-cmd-norm[%i] %f\n", i, throttleI);
        sb.append(&tmp);
    }
    sb.append("iterate 1\n");
    */
    for (int i = 0; i < 4; i++) {
        String sb = String::format("set fcs/throttle-cmd-norm[%i] %f\n", i, 1.0f);
        int sent = con->send(&sb);
    }
}
void resume(Socket *socket) {
    String msg = String("resume\n");
    socket->send(&msg);
}

int main(int argc, char **argv) {

    SocketFactory *fac = new a8::hal::socket::native::NativeSocketFactory();
    a8::util::Finally releaseSocket([&]() {
        S->out->println("Going to delete socket factory.");
        delete fac;
    });

    String host = "127.0.0.1";
    int port = 5126;

    Socket *socket = fac->socket();
    socket->connect(host, port);

    if (!socket) {
        return -1;
    }

    a8::util::Finally closeSocket([&]() {
        delete socket;
    });

    // resume(socket);

    sendServo(socket);
    int loops = 0;
    while (true) {
        loops++;
        if (loops == 2) {
            // sendServo(socket);
        }
        char buf[100];
        // socket->send("\n", 1);
        // S->out->print("Receiving message...");
        int len = socket->receive(buf, 100);
        // S->out->print("Processing message received...");
        if (len <= 0) {
            if (len == 0) {
                S->out->println("Socket connect closed by server.");
            } else {
                int error = WSAGetLastError();
                S->out->println(String::format("Failed with error:%i", error));
            }
            break; // end loop
        }

        String received = String(buf, len);
        if (received.endWith("JSBSim>")) {
            // sendServo(socket);
        }
        S->out->println(":" + received);
    }
    //
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
