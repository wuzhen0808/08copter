#pragma once
namespace a8::core {

class Configuration {

public:
    virtual int getServoAttachPin(int servoId) ;
};
} // namespace a8::core