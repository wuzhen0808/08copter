#pragma once
namespace a8::hal {

class Servo {
private:
    int id;

public:
    Servo(int id) {
        this->id = id;
    }
    int getId() {
        return this->id;
    }
    virtual void setup() = 0;
    virtual void writeMicroseconds(int pw) = 0;
};

} // namespace a8::hal