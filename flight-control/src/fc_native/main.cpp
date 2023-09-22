#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosInitializer.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/native/NativeCopter.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/hal/native/socket/NativeSocket.h"
#include "a8/util/Finally.h"
#include "a8/util/Result.h"

#include <iostream>
#include <stdio.h>

using a8::freertos::FreeRtosInitializer;
using a8::freertos::FreeRtosScheduler;

using a8::core::Copter;
using a8::core::Scheduler;

using a8::hal::native::NativeCopter;
using a8::hal::native::NativeSystem;
using a8::hal::native::socket::NativeSocket;
using a8::hal::socket::Socket;
using a8::hal::socket::SocketFactory;
using a8::util::String;

System *a8::hal::S = new NativeSystem();

// int main_blinky(int argc, char **argv) {

/**
 * see lib/native/src/main.c
 */

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

int mainx(int argc, char **argv) {

    SocketFactory *fac = new a8::hal::native::socket::NativeSocketFactory();
    a8::util::Finally releaseSocket([&]() {
        S->out->println("Going to delete socket factory.");
        delete fac;
    });

    Socket *socket = fac->socket();

    String host = "127.0.0.1";
    int port = 5126;

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

int main(int argc, char **argv) {

    // S->out->println("Hello!");
    // FreeRtosInitializer * rtos = new FreeRtosInitializer();
    // rtos->initialize();
    // S->out->println("RTOS initialized!");
    SocketFactory *sFac = new a8::hal::native::socket::NativeSocketFactory();
    Socket *socket = sFac->socket();
    Scheduler *scheduler = new FreeRtosScheduler();
    Copter *copter = new NativeCopter(scheduler, sFac, "127.0.0.1", 5126);

    Result rst = copter->setup();
    if (rst.error) {
        cout << "failed to setup the copter, the error message:" << rst.message.getText() << endl;
        return rst.error;
    }
    rst = copter->start();
    if (rst.error) {
        cout << "failed to startup the copter, the error message:" << rst.message.getText() << endl;
        return rst.error;
    }

    cout << "Hello,main()";
    return 0;
}
