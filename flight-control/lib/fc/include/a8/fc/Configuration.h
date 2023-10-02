#pragma once
namespace a8::fc {

class Configuration {

public:
    virtual int getServoAttachPin(int servoId) ;
};
} // namespace a8::fc