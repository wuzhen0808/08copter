#ifndef ATTITUDE_SENSOR__
#define ATTITUDE_SENSOR__
class AttitudeSensor {
public:
    virtual void update() = 0;
    virtual float getRoll() = 0;
    virtual float getPitch() = 0;
    virtual float getYaw() = 0;
};
#endif