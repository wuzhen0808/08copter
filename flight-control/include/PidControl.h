#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__
// Reference: 
// https://github.com/bitcraze/crazyflie-firmware.git

class PidControl
{
public:
    PidControl(float kp, float ki, float kd);
    ~PidControl();
    float update(float desirePosition, float actualPosition);

private:
    float kp;
    float ki;
    float kd;        
    unsigned long lastUpdateTimeInMs;
    float lastError;

};
#endif
