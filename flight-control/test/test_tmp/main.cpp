
#include "a8/hal/Hal.h"
#include "a8/hal/System.h"
#include "a8/hal/native/NativeApplication.h"
#include "a8/hal/native/NativeSystem.h"
#include <iostream>

using namespace std;

using namespace a8::hal;
using namespace a8::hal::native;

System *a8::hal::S = new NativeSystem();

int receive(Sockets &ss, int port, char *buf, int bufLen);

int main(int argc, char **argv) {
    Application *app = new NativeApplication();
    app->start();

    return 0;
}

int receive(Sockets &ss, int port, char *buf, int bufLen) {
    SOCK server = ss.socket();
    if (server == 0) {
        // context.stop(String::format("cannot create new socket from socket factory."));
        return false;
    }
    String address = "127.0.0.1";

    Result bResult = ss.bind(server, address, port); //
    if (bResult.error != 0) {
        // context.stop(String::format("cannot bind to port:%i, error:%i", bindPort, bResult.error));
        return false;
    }

    Result lResult = ss.listen(server);

    if (lResult.error != 0) {
        // context.stop(String::format("cannot listen on port:%i", bindPort));
        return false;
    }

    // log(String::format("Waiting the JSBSim start up and connect to the the address:%s:%i", address.getText(), bindPort));

    SOCK sockIn = ss.accept(server);

    if (sockIn == 0) {
        return false;
    }
    cout << "connect in" << endl;
    SocketReader *sr = new SocketReader(&ss, sockIn);
    LineReader *lr = new LineReader(sr);
    String line;
    while (true) {
        line = "";
        bool more = lr->readLine(line);
        cout << line.getText() << endl;

        if (more) {
            // continue;
        }
    }
    for (int i = 0;; i++) {
        // int ret = ss.receive(sockIn, buf, bufLen);
        int ret = sr->read(buf, bufLen);
        if (ret == 0) {
            break;
        } else if (ret < 0) {
            break;
        }
        String str(buf, ret);
        cout << str.getText() << endl;
    }
    return false;
}