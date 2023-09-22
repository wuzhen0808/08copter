#include "a8/hal/native/NativeServosControl.h"
#include "a8/hal/native/NativeServo.h"
#include "a8/hal/native/socket/SocketReader.h"
namespace a8::hal::native {
using a8::hal::socket::Socket;
using a8::hal::native::socket::SocketReader;

void setupTheSocketReaderThread(Scheduler *scheduler) {
    
}
NativeServosControl::NativeServosControl(int totalServos, Socket *socket, String host, int port) : ServosControl(totalServos) {
    this->socket = socket;
    this->host = host;
    this->port = port;
}

NativeServosControl::~NativeServosControl() {

}

Result NativeServosControl::setup(){
    Result connected = this->socket->connect(host, port);
    if (connected.error) {
        return connected;
    }

    this->add(new SocketReader(this->socket));

    ServosControl::setup();

    return true;
}
Servo *NativeServosControl::setupServo(int servoId) {
    // return new NativeServo();
    return new NativeServo(socket, servoId);
}

} // namespace a8::hal::native