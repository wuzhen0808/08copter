#pragma once
#include "a8/util.h"
#include "a8/fc/collect/Collector.h"
#define A8_PM_DEBUG (0)
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;

class PowerManage : public FlyWeight {
    int voltagePin;
    StyledLed *led;
    System *sys;
    float voltage;
    float remindVoltage = 11.0f;
    float dangerVoltage = 10.0f;

    long lastUpdateTimeMs = -1;
    long lastLedSetTimeMs = -1;

public:
    PowerManage(System *sys, int voltagePin, int ledPin, LoggerFactory *logFac) : FlyWeight(logFac, "PowerManage") {
        this->sys = sys;
        this->voltagePin = voltagePin;
        this->led = new StyledLed(sys, ledPin);
        this->voltage = -1.0f;
    }

    ~PowerManage() {
        delete this->led;
    }

    void setup() {
        this->led->setup();
        this->led->start(sys->getSteadyTime());
    }
    void collectDataItems(Collector &collector) {
        collector.add<float>("voltage", this->voltage);
    }
    void tick(long timeMs) {

        if (this->lastUpdateTimeMs + 1000 < timeMs) {
            this->update(timeMs);
        }
        if (this->lastLedSetTimeMs < this->lastUpdateTimeMs) {
            if (this->voltage < dangerVoltage) {
                if (A8_PM_DEBUG) {
                    log("danger");
                }
                led->danger();
            } else if (this->voltage < this->remindVoltage) {
                if (A8_PM_DEBUG) {
                    log("warn");
                }
                led->warn(); // warn mode.
            } else {
                if (A8_PM_DEBUG) {
                    log("info");
                }
                led->info(); // normal mode.
            }
            this->lastLedSetTimeMs = timeMs;
        }
        Result res;
        int ret = led->tick(timeMs, res);

        if (ret < 0) {
            log(String() << "led error(" << ret << "),detail:" << res.errorMessage);
        }
        if (A8_PM_DEBUG) {
            log(String() << "led tick return:" << ret);
        }
    }
    // TODO thread safe.
    void update(long timeMs) {
        update(timeMs, false);
    }

    void update(long timeMs, bool debug) {
        int mV = sys->analogRead(voltagePin); /// 1024.0f * 12.0f;
        voltage = mV * 1e-3 * 4.10f;
        if (debug) {
            log(String() << "raw:" << mV << "voltage:" << voltage);
        }

        this->lastUpdateTimeMs = timeMs;
    }

    int isReady(Result &res) {
        long timeMs = sys->getSteadyTime();
        this->update(timeMs);
        if (this->voltage < dangerVoltage) {
            res << "voltage(" << voltage << ") is low, it must higher than " << dangerVoltage << ".";
            return -1;
        }
        return 1;
    }

    float getVoltage() {
        return voltage;
    }
};
} // namespace a8::fc