#pragma once
namespace a8::hal {

class Servo {
public:
    virtual void writeMicroseconds(int pw) = 0;
    virtual void attach(int pin) = 0;
};

} // namespace a8::hal