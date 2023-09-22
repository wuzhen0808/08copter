#pragma once
#include "a8/hal/Servo.h"
#include "a8/hal/Socket.h"
namespace a8::hal::native {
using a8::hal::socket::Socket;

class NativeServo : public a8::hal::Servo {
private:
    Socket *socket_;
    int id;

public:
    NativeServo(Socket *socket, int id) : Servo(id) {
        this->socket_ = socket;
        this->id = id;
    }
    ~NativeServo() {}

    virtual void writeMicroseconds(int pw);
    virtual void setup();
};

} // namespace a8::hal::native
