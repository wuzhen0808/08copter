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
    long timeMsPrev;
    long timeMs;
    float elapsedTimeSec;
    float desired_angle = .0f;
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
        this->pidRoll = new Pid();
        this->pidPitch = new Pid();
    }

    void setTimeLimitMs(long ms) {
        this->timeLimitMs = ms;
    }

    void start() {
        this->startTimeMs = sys->getSteadyTime();
    }

    int update(Result &res) {
        if (this->startTimeMs < 0) {
            res << "not started.";
            return -1;
        }

        float roll;
        float pitch;
        float yaw;
        int ret = rpy->getRollPitchYaw(roll, pitch, yaw);
        if (!ret < 0) {
            return ret;
        }

        timeMsPrev = timeMs;
        timeMs = sys->getSteadyTime();
        elapsedTimeSec = (timeMs - timeMsPrev) / 1000.0f;
        String msg;
        msg << "\trpy:" << roll << "," << pitch << "," << yaw << ")";

        // pid
        float errorRoll = desired_angle - roll;
        pidRoll->update(errorRoll, elapsedTimeSec);

        float errorPitch = desired_angle - pitch;
        pidPitch->update(errorPitch, elapsedTimeSec);
        //
        long pwmRoll = pidRoll->getPwm();
        long pwmPitch = pidPitch->getPwm();
        msg << "error:(" << errorRoll << "," << errorPitch << ")pidRoll:(" << pwmRoll << "," << pwmPitch << ")";

        long pwmLH = throttle + pwmRoll + pwmPitch;
        long pwmRH = throttle - pwmRoll + pwmPitch;
        long pwmLA = throttle + pwmRoll - pwmPitch;
        long pwmRA = throttle - pwmRoll - pwmPitch;

        /*Once again we map the PWM values to be sure that we won't pass the min
        and max values. Yes, we've already maped the PID values. But for example, for
        throttle value of 1300, if we sum the max PID value we would have 2300us and
        that will mess up the ESC.*/
        // Right
        limitPwm2(pwmLH, pwmRH, pwmLA, pwmRA);

        msg << "\tpwm:(" << pwmLH << "," << pwmRH << "," << pwmLA << "," << pwmRA << ")";

        // apply pwm
        this->propellerLH->setThrottlePwm(pwmLH);
        this->propellerRH->setThrottlePwm(pwmRH);
        this->propellerLA->setThrottlePwm(pwmLA);
        this->propellerRA->setThrottlePwm(pwmRA);

        log(msg << "\n");
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