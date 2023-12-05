#pragma once
#include "a8/fc/Pid.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class Balancer : public FlyWeight {

    Rpy *rpy;
    Propeller *propellerLH; // left head.
    Propeller *propellerRH; // right head.
    Propeller *propellerLA; // left after.
    Propeller *propellerRA; // right after.
    System *sys;
    Pid *pidRoll;
    Pid *pidPitch;

    long timeMs;
    float desiredRoll = .0f;
    float desiredPitch = .0f;
    float desiredYaw = .0f;
    long throttle = 1230;

    long timeLimitMs = -1;
    long startTimeMs = -1;
    long pwmMin = 1000;
    long pwmMax = 2000;

public:
    Balancer(Propeller *lh, Propeller *rh, Propeller *la, Propeller *ra, Rpy *rpy, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, "Balancer") {
        this->propellerLH = lh;
        this->propellerRH = rh;
        this->propellerLA = la;
        this->propellerRA = ra;
        this->rpy = rpy;
        this->sys = sys;
        this->pidRoll = new Pid(this->timeMs);
        this->pidPitch = new Pid(this->timeMs);
    }
    bool mpuUpdate() {
        return rpy->update();
    }
    float getRoll() {
        return rpy->getRoll();
    }
    float getPitch() {
        return rpy->getPitch();
    }

    float getYaw() {
        return rpy->getYaw();
    }

    void setTimeLimitMs(long ms) {
        this->timeLimitMs = ms;
    }

    void start() {
        this->startTimeMs = sys->getSteadyTime();
        this->timeMs = this->startTimeMs;
    }

    int update(Result &res) {

        if (this->startTimeMs < 0) {
            res << "not started.";
            return -1;
        }
        bool ok = rpy->update();
        if (!ok) {
            return -1;
        }
        this->timeMs = sys->getSteadyTime();

        float roll = rpy->getRoll();
        float pitch = rpy->getPitch();

        float yaw = rpy->getYaw();

        // pid

        pidRoll->update(roll, desiredRoll);
        pidPitch->update(pitch, desiredPitch);

        String msg;
        msg << "rpy:(" << roll << "," << pitch << "," << yaw << ")";
        //
        long pwmRoll = pidRoll->getPwm();
        long pwmPitch = pidPitch->getPwm();
        
        //========================================================================================
        //TODO there is a wired problem:
        // 1) if you uncomment the following line.
        //  log("abc")
        // 2) you will see the MPU9250 stop working with the roll,pitch is printed as below:
        //  (0.2147483687e--2147483687,0.2147483687e--2147483687,-0.751e1)
        // 3)if you print the value with arduino's Serial.print, below output :
        //  (0.00,0.00,-7.51)        
        //========================================================================================

        long pwmLH = throttle + pwmRoll + pwmPitch;
        long pwmRH = throttle - pwmRoll + pwmPitch;
        long pwmLA = throttle + pwmRoll - pwmPitch;
        long pwmRA = throttle - pwmRoll - pwmPitch;

        limitPwm2(pwmLH, pwmRH, pwmLA, pwmRA);

        // Right

        msg << "\tpwm:(" << pwmLH << "," << pwmRH << "," << pwmLA << "," << pwmRA << ")";

        // apply pwm
        this->propellerLH->setThrottlePwm(pwmLH);
        this->propellerRH->setThrottlePwm(pwmRH);
        this->propellerLA->setThrottlePwm(pwmLA);
        this->propellerRA->setThrottlePwm(pwmRA);
        /*
         */
        log(msg); // TODO why ?
        return 1;
    }

    int update2(String &msg, float roll, float pitch, float yaw, long pwmRoll, long pwmPitch) {

        /*Once again we map the PWM values to be sure that we won't pass the min
        and max values. Yes, we've already maped the PID values. But for example, for
        throttle value of 1300, if we sum the max PID value we would have 2300us and
        that will mess up the ESC.*/
        return 1;
    }

    void limitPwm(long &pwm) {
        if (pwm < pwmMin) {
            pwm = pwmMin;
        } else if (pwm > pwmMax) {
            pwm = pwmMax;
        }
    }

    void limitPwm2(long &pwmLH, long &pwmRH, long &pwmLA, long &pwmRA) {
        limitPwm(pwmLH);
        limitPwm(pwmRH);
        limitPwm(pwmLA);
        limitPwm(pwmRA);

        if (timeLimitMs > 0 && timeMs - startTimeMs > timeLimitMs) {
            log("timeLimitSeconds exceed, stop all propellers.");
            pwmLH = pwmMin;
            pwmRH = pwmMin;
            pwmLA = pwmMin;
            pwmRA = pwmMin;
        }
    }
};
} // namespace a8::fc