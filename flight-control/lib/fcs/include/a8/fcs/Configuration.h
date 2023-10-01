#pragma once
namespace a8::fcs {

class Configuration {

public:
    virtual int getServoAttachPin(int servoId) ;
};
} // namespace a8::fcs