#pragma once
#include "a8/fc/Pid.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/Throttler.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

class BalanceThrottler : public Throttler {

    Rpy *rpy;
    System *sys;
    Pid *pidRoll;
    Pid *pidPitch;

    long timeMs;
    float desiredRoll = .0f;
    float desiredPitch = .0f;
    float desiredYaw = .0f;

    long timeLimitMs = -1;
    long startTimeMs = -1;

public:
    BalanceThrottler(Rpy *rpy, System *sys, LoggerFactory *logFac) : Throttler(logFac, "BalanceThrottler") {

        this->rpy = rpy;
        this->sys = sys;
        this->pidRoll = new Pid(this->timeMs);
        this->pidPitch = new Pid(this->timeMs);
    }

    void setTimeLimitMs(long ms) {
        this->timeLimitMs = ms;
    }

    int update(Context &ctx, Result &res) override {
        bool ok = rpy->update();
        if (!ok) {
            res << "failed to update rpy.";
            return -1;
        }

        float roll = rpy->getRoll();
        float pitch = rpy->getPitch();

        float yaw = rpy->getYaw();
        ctx << (String() << "rpy:(" << roll << "," << pitch << "," << yaw << ")");
        // pid

        pidRoll->update(roll, desiredRoll, ctx.message);
        pidPitch->update(pitch, desiredPitch, ctx.message);

        //
        long pwmRoll = pidRoll->getPwm();
        long pwmPitch = pidPitch->getPwm();
        pwmRoll /= 2;
        pwmPitch /= 2;

        //========================================================================================
        // TODO there is a wired problem:
        // 1) if you uncomment the following line.
        //  log("abc")
        // 2) you will see the MPU9250 stop working with the roll,pitch is printed as below:
        //  (0.2147483687e--2147483687,0.2147483687e--2147483687,-0.751e1)
        // 3)if you print the value with arduino's Serial.print, below output :
        //  (0.00,0.00,-7.51)
        //========================================================================================

        long pwmLH = 0 + pwmRoll + pwmPitch;
        long pwmRH = 0 - pwmRoll + pwmPitch;
        long pwmLA = 0 + pwmRoll - pwmPitch;
        long pwmRA = 0 - pwmRoll - pwmPitch;
        ctx.addPwm(pwmLH, pwmRH, pwmLA, pwmRA);
        return 1;
    }
};
} // namespace a8::fc