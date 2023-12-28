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

    int balanceMode;

    float roll;
    float pitch;
    float yaw;

public:
    BalanceThrottler(Rpy *rpy, int bMode, LoggerFactory *logFac) : Throttler(logFac, "BalanceThrottler") {
        this->rpy = rpy;
        this->pidRoll = new Pid(logFac, "RollPid");
        this->pidPitch = new Pid(logFac, "PitchPid");
        this->balanceMode = bMode;
    }
    ~BalanceThrottler() {
        delete this->pidRoll;
        delete this->pidPitch;
    }

    void setup() override {
        this->pidRoll->setup();
        this->pidPitch->setup();
    }

    int collectDataItems(Collector *collector, Result &res) override {
        int ret = 1;
        if (ret > 0)
            ret = collector->add("roll", this->roll, res);
        if (ret > 0) {
            ret = collector->add("pitch", this->pitch, res);
        }
        if (ret > 0) {
            ret = collector->add("yaw", this->yaw, res);
        }
        if (ret > 0) {
            ret = this->pidRoll->collectDataItems(collector, res);
        }
        if (ret > 0) {
            ret = this->pidPitch->collectDataItems(collector, res);
        }
        return ret;
    }
    void getLimitInTheory(float &minSample, float &maxSample) override {
        minSample = minSample - pidRoll->getOutputLimit() - pidPitch->getOutputLimit();
        maxSample = maxSample + pidRoll->getOutputLimit() + pidPitch->getOutputLimit();
    }

    void setPidArgument(double kp, double ki, double kd, float outputLimit, float maxPidIntegralOutput) {
        this->pidRoll->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
        this->pidPitch->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
    }

    int update(Throttle &ctx, Result &res) override {
        A8_LOG_DEBUG(logger, ">>Bal.update.");

        rpy->get(roll, pitch, yaw);
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
        if (balanceMode == BalanceMode::ROLL) {
            pitchThrottle = 0;
        } else if (balanceMode == BalanceMode::PITCH) {
            rollThrottle = 0;
        }

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