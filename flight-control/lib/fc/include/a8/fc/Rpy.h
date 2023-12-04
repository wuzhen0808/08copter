#pragma once
namespace a8::fc {
class Rpy {
public:
    virtual int getRollPitchYaw(float &roll, float &pitch, float &yaw) = 0;
};
} // namespace a8::fc