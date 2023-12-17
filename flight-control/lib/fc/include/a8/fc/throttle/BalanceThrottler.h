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

    long maxBalancePwm = 0;

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
    void getLimitInTheory(long &minSample, long &maxSample) override {
        minSample = minSample - pidRoll->getOutputLimit() - pidPitch->getOutputLimit();
        maxSample = maxSample + pidRoll->getOutputLimit() + pidPitch->getOutputLimit();
    }

    void printHistory(int depth, String &msg) override {
        msg << StringUtil::space(depth) << "Balance-history-rollPid:\n";
        this->pidRoll->printHistory(depth + 1, msg);
        msg << StringUtil::space(depth) << "Balance-history-pitchPid:\n";
        this->pidPitch->printHistory(depth + 1, msg);
    }
    void setPidArgument(double kp, double ki, double kd, float outputLimit, float maxPidIntegralOutput) {
        this->pidRoll->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
        this->pidPitch->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
    }

    int update(Context &ctx, Result &res) override {
        A8_LOG_DEBUG(logger, ">>Bal.update.");
        bool ok = rpy->update();
        if (!ok) {
            res << "failed to update rpy.";
            A8_LOG_DEBUG(logger, "<<Bal.update.failed");
            return -1;
        }
        A8_LOG_DEBUG(logger, ">>Bal.update.1");
        float roll = rpy->getRoll();
        A8_LOG_DEBUG(logger, ">>Bal.update.2");
        float pitch = rpy->getPitch();
        A8_LOG_DEBUG(logger, ">>Bal.update.3");
        float yaw = rpy->getYaw();
        A8_LOG_DEBUG(logger, ">>Bal.update.31");
        ctx << (String() << "rpy:(");
        A8_LOG_DEBUG(logger, ">>Bal.update.32");
        ctx << (String()<< roll << "," << pitch << "," << yaw << ")");
        // pid
        A8_LOG_DEBUG(logger, ">>Bal.update.5");
        pidRoll->update(ctx.timeMs_, roll, desiredRoll, ctx.message);
        A8_LOG_DEBUG(logger, ">>Bal.update.6");
        pidPitch->update(ctx.timeMs_, pitch, desiredPitch, ctx.message);
        A8_LOG_DEBUG(logger, ">>Bal.update.8");
        //
        long pwmRoll = pidRoll->getPwm();
        long pwmPitch = pidPitch->getPwm();
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

        long pwmLH = 0 + pwmRoll + pwmPitch;
        long pwmRH = 0 - pwmRoll + pwmPitch;
        long pwmLA = 0 + pwmRoll - pwmPitch;
        long pwmRA = 0 - pwmRoll - pwmPitch;
        A8_LOG_DEBUG(logger, ">>Bal.update.9");
        ctx.propellers->addPwm(pwmLH, pwmRH, pwmLA, pwmRA);
        A8_LOG_DEBUG(logger, "<<Bal.update.2");
        return 1;
    }
};
} // namespace a8::fc::throttle