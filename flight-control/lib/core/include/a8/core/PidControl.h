#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__
// Reference:
// https://github.com/bitcraze/crazyflie-firmware.git
namespace a8 {
namespace core {
class PidControl {

private:
    unsigned long lastUpdateTimeInMs = 0;
    float lastError;

public:
    float kp;
    float ki;
    float kd;
    
    PidControl(float kp, float ki, float kd);
    ~PidControl();

    float update(float desirePosition, float actualPosition);
    void setup(float kp, float ki, float kd) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
    }
};
} // namespace core
} // namespace a8
#endif
