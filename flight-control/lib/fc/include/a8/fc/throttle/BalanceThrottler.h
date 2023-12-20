#pragma once
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/throttle/Pid.h"
#include "a8/fc/throttle/Throttler.h"
#include "a8/util.h"

namespace a8::fc::throttle {
using namespace a8::util;

class BalanceThrottler : public Throttler {

    Rpy *rpy;
    Pid *pidRoll;
    Pid *pidPitch;

    float desiredRoll = .0f;
    float desiredPitch = .0f;
    float desiredYaw = .0f;

    long startTimeMs = -1;

    float maxBalanceThrottle = 0;

public:
    BalanceThrottler(Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, "BalanceThrottler") {

        this->rpy = rpy;
        this->pidRoll = new Pid(logFac, "RollPid");
        this->pidPitch = new Pid(logFac, "PitchPid");
    }
    ~BalanceThrottler() {
        delete this->pidRoll;
        delete this->pidPitch;
    }
    void getLimitInTheory(float &minSample, float &maxSample) override {
        minSample = minSample - pidRoll->getOutputLimit() - pidPitch->getOutputLimit();
        maxSample = maxSample + pidRoll->getOutputLimit() + pidPitch->getOutputLimit();
    }

    void printHistory(int depth, History &his) override {
        his.add(depth, "Balance-history-rollPid:");
        this->pidRoll->printHistory(depth + 1, his);
        his.add(depth, "Balance-history-pitchPid:");
        this->pidPitch->printHistory(depth + 1, his);
    }
    void setPidArgument(double kp, double ki, double kd, float outputLimit, float maxPidIntegralOutput) {
        this->pidRoll->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
        this->pidPitch->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
    }

    int update(Throttle &ctx, Result &res) override {
        A8_LOG_DEBUG(logger, ">>Bal.update.");
        bool ok = rpy->update();
        if (!ok) {
            res << "failed to update rpy.";
            A8_LOG_DEBUG(logger, "<<Bal.update.failed");
            return -1;
        }
        float roll = rpy->getRoll();
        float pitch = rpy->getPitch();
        float yaw = rpy->getYaw();        
        // pid
        pidRoll->update(ctx.timeMs_, roll, desiredRoll);
        pidPitch->update(ctx.timeMs_, pitch, desiredPitch);
        //
        float rollThrottle = pidRoll->getOutput();
        float pitchThrottle = pidPitch->getOutput();
        // pwmRoll /= 2;
        // pwmPitch /= 2;

        //========================================================================================
        // TODO there is a wired problem:
        // 1) if you uncomment the following line.
        //  log("abc")
        // 2) you will see the MPU9250 stop working with the roll,pitch is printed as below:
        //  (0.2147483687e--2147483687,0.2147483687e--2147483687,-0.751e1)
        // 3)if you print the value with arduino's Serial.print, below output :
        //  (0.00,0.00,-7.51)
        //========================================================================================

        float thLH = 0 + rollThrottle + pitchThrottle;
        float thRH = 0 - rollThrottle + pitchThrottle;
        float thLA = 0 + rollThrottle - pitchThrottle;
        float thRA = 0 - rollThrottle - pitchThrottle;
        A8_LOG_DEBUG(logger, ">>Bal.update.9");
        ctx.propellers->addThrottle(thLH, thRH, thLA, thRA);
        A8_LOG_DEBUG(logger, "<<Bal.update.2");
        return 1;
    }
};
} // namespace a8::fc::throttle