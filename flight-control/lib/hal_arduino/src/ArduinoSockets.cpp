#include "a8/hal/arduino/ArduinoSockets.h"
namespace a8::hal::arduino_ {

ArduinoSockets::ArduinoSockets() {
}
ArduinoSockets::~ArduinoSockets() {
}

void ArduinoSockets::close(SOCK &sock) {
}
/*
 Result setBlocking(SOCK sock, bool blocking) {
    u_long mode = blocking ? 0 : 1;
    int ret = ioctlsocket(sock, FIONBIO, &mode);
    if (ret == 0) {
        return true;
    }

    return ret;
}
*/

int ArduinoSockets::connect(SOCK sock, const String host, int port, Result &res) {
    return true;
}

int ArduinoSockets::bind(SOCK sock, const String address, int port, Result &res) {
    return -1;
}

int ArduinoSockets::listen(SOCK sock, Result &rst) {
    return -1;
}

int ArduinoSockets::accept(SOCK sock, SOCK &sockIn) {
    return -1;
}

bool ArduinoSockets::send(SOCK sock, const char *buf, int len) {
    return -1;
}

int ArduinoSockets::receive(SOCK sock, char *buf, int bufLen) {
    return -1;
}

int ArduinoSockets::getLastError() {
    return -1;
}
bool ArduinoSockets::isReady() {
    return -1;
}
int ArduinoSockets::socket(SOCK &sock) {
    return -1;
}

int ArduinoSockets::socket(SOCK &sock, Result &res) {
    return -1;
}

int ArduinoSockets::select(SOCK &sock) {
    return -1;
}
int ArduinoSockets::select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) {
    return -1;
}
} // namespace a8::hal::arduino_