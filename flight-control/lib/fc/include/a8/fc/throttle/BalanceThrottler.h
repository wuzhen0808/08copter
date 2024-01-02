#pragma once
#include "a8/fc/Factory.h"
#include "a8/fc/Propeller.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/config/FlightConfigItem.h"
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

    float rpy_[3];
    float fRpy_[3];

    FlightConfigItem *config;

public:
    BalanceThrottler(FlightConfigItem *config, Factory *fac, Rpy *rpy, LoggerFactory *logFac) : Throttler(logFac, "BalanceThrottler") {
        this->config = config;
        this->rpy = rpy;
        this->rollPid = new Pid(logFac, "RollPid");
        this->pitchPid = new Pid(logFac, "PitchPid");
        this->yawPid = new Pid(logFac, "YawPid");

        if (config->pidEnableErrorDiffFilter) {
            Rate tickRate = Rate::ms(config->tickTimeMs);
            int order = config->pidErrorDiffFilterOrder;
            this->rollPid->setErrorDiffFilter(fac->newLowPassFilter(config->pidErrorDiffFilterCutOffRate, tickRate, order), Lang::delete_<Filter>);
            this->pitchPid->setErrorDiffFilter(fac->newLowPassFilter(config->pidErrorDiffFilterCutOffRate, tickRate, order), Lang::delete_<Filter>);
            this->yawPid->setErrorDiffFilter(fac->newLowPassFilter(config->pidErrorDiffFilterCutOffRate, tickRate, order), Lang::delete_<Filter>);
        }
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

        rpy->get(rpy_, fRpy_);
        // pid
        rollPid->update(ctx.timeMs_, rpy_[0], fRpy_[0], desiredRoll);
        pitchPid->update(ctx.timeMs_, rpy_[1], fRpy_[1], desiredPitch);
        yawPid->update(ctx.timeMs_, rpy_[2], fRpy_[2], desiredYaw);
        //
        float rollThrottle = rollPid->getOutput();
        float pitchThrottle = pitchPid->getOutput();
        float yawThrottle = yawPid->getOutput();
        if (config->balanceMode == BalanceMode::ROLL) {
            pitchThrottle = 0;
            yawThrottle = 0;
        } else if (config->balanceMode == BalanceMode::PITCH) {
            rollThrottle = 0;
            yawThrottle = 0;
        } else if (config->balanceMode == BalanceMode::YAW) {
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