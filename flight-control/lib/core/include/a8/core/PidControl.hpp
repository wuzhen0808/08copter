#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__
// Reference:
// https://github.com/bitcraze/crazyflie-firmware.git
namespace a8 {
namespace core {
class PidControl {
public:
    PidControl(float kp, float ki, float kd);
    ~PidControl();
    float update(float desirePosition, float actualPosition);

private:
    float kp;
    float ki;
    float kd;
    unsigned long lastUpdateTimeInMs = 0;
    float lastError;
};
} // namespace core
} // namespace a8
#endif
