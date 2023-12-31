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
    Pid *rollPid;
    Pid *pitchPid;
    Pid *yawPid;

    float desiredRoll = .0f;
    float desiredPitch = .0f;
    float desiredYaw = .0f;

    long startTimeMs = -1;

    float maxBalanceThrottle = 0;

    int balanceMode;

    float rpy_[3];

public:
    BalanceThrottler(Rpy *rpy, int bMode, int errDiffMaWidth, LoggerFactory *logFac) : Throttler(logFac, "BalanceThrottler") {
        this->rpy = rpy;
        this->rollPid = new Pid(logFac, "RollPid", errDiffMaWidth);
        this->pitchPid = new Pid(logFac, "PitchPid", errDiffMaWidth);
        this->yawPid = new Pid(logFac, "YawPid", errDiffMaWidth);
        this->balanceMode = bMode;
    }
    ~BalanceThrottler() {
        delete this->rollPid;
        delete this->pitchPid;
        delete this->yawPid;
    }

    void setup() override {
        this->rollPid->setup();
        this->pitchPid->setup();
        this->yawPid->setup();
    }

    int collectDataItems(Collector *collector, Result &res) override {
        int ret = 1;
        if (ret > 0)
            ret = collector->add("roll", this->rpy_[0], res);
        if (ret > 0) {
            ret = collector->add("pitch", this->rpy_[1], res);
        }
        if (ret > 0) {
            ret = collector->add("yaw", this->rpy_[2], res);
        }
        if (ret > 0) {
            ret = this->rollPid->collectDataItems(collector, res);
        }
        if (ret > 0) {
            ret = this->pitchPid->collectDataItems(collector, res);
        }
        if (ret > 0) {
            ret = this->yawPid->collectDataItems(collector, res);
        }

        return ret;
    }

    void setPidArgument(double kp, double ki, double kd, float outputLimit, float maxPidIntegralOutput) {
        this->rollPid->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
        this->pitchPid->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
    }

    void setYawPidArgument(double kp, double ki, double kd, float outputLimit, float maxPidIntegralOutput) {
        this->yawPid->config(kp, ki, kd, outputLimit, maxPidIntegralOutput);
    }

    int update(Throttle &ctx, Result &res) override {
        A8_LOG_DEBUG(logger, ">>Bal.update.");

        rpy->get(rpy_);
        // pid
        rollPid->update(ctx.timeMs_, rpy_[0], desiredRoll);
        pitchPid->update(ctx.timeMs_, rpy_[1], desiredPitch);
        yawPid->update(ctx.timeMs_, rpy_[2], desiredYaw);
        //
        float rollThrottle = rollPid->getOutput();
        float pitchThrottle = pitchPid->getOutput();
        float yawThrottle = yawPid->getOutput();
        if (balanceMode == BalanceMode::ROLL) {
            pitchThrottle = 0;
            yawThrottle = 0;
        } else if (balanceMode == BalanceMode::PITCH) {
            rollThrottle = 0;
            yawThrottle = 0;
        } else if (balanceMode == BalanceMode::YAW) {
            rollThrottle = 0;
            pitchThrottle = 0;
        }

        float thLH = yawThrottle - rollThrottle - pitchThrottle;
        float thRH = -yawThrottle + rollThrottle - pitchThrottle;
        float thLA = -yawThrottle - rollThrottle + pitchThrottle;
        float thRA = yawThrottle + rollThrottle + pitchThrottle;

        A8_LOG_DEBUG(logger, ">>Bal.update.9");
        ctx.propellers->addThrottle(thLH, thRH, thLA, thRA);
        A8_LOG_DEBUG(logger, "<<Bal.update.2");
        return 1;
    }
};
} // namespace a8::fc::throttle