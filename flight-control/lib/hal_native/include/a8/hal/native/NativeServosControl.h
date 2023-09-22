#pragma once
#include "a8/core/Copter.h"
#include "a8/core/ServosControl.h"
#include "a8/hal/Servo.h"
#include "a8/hal/Socket.h"

namespace a8::hal::native {
using namespace a8::core;
using a8::hal::socket::Socket;

class NativeServosControl : public ServosControl {

private:
    Socket* socket;
    String host;
    int port;
    
public:
    NativeServosControl(int totalServos, Socket* socket, String host, int port);
    ~NativeServosControl();
    virtual Result setup();
    virtual Servo *setupServo(int servoId);
};

} // namespace a8::arduino
