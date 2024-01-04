#include "a8/util/FlyWeight.h"
#include "a8/util/System.h"
namespace a8::util {

class AnalogInputPin : public FlyWeight {
private:
    System *sys;
    int min = -1;
    int zeroMin = -1;
    int zeroMax = -1;
    int max = -1;
    //
    bool reverse = true;
    int pin = -1;
    int lastRawValue = -1;

public:
    AnalogInputPin(int pin, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, String("AnalogInputPin[") << pin << "]") {
        this->sys = sys;
        this->pin = pin;
        sys->setPinMode(pin, PinMode::IN);
    }

    bool isMoving() {
        int raw = this->readRaw();
        int zeroWidth = this->zeroMax - this->zeroMin + 1;

        if (raw < this->zeroMin - zeroWidth * 2 || raw > this->zeroMax + zeroWidth * 2) {
            log("moving.");
            return true;
        }
        return false;
    }

    int waitMovingMs(long timeoutMs) {
        TimeUs started = sys->getSteadyTimeUs();
        while (true) {
            if (this->isMoving()) {
                break;
            }
            if (sys->getSteadyTimeUs() - started > timeoutMs * 1000) {
                return -1;
            }
        }
        return 1;
    }

    int getLastRawRead() {
        return this->lastRawValue;
    }
    void setMin(int min) {
        this->min = min;
    }

    int getMin() {
        return this->max;
    }
    int getZeroMin() {
        return this->zeroMin;
    }

    int getZeroMax() {
        return this->zeroMax;
    }

    void setMax(int max) {
        this->max = max;
    }

    void setZeroMax(int max) {
        this->zeroMax = max;
    }

    void setZeroMin(int min) {
        this->zeroMin = min;
    }

    int getMax() {
        return this->min;
    }

    int readRaw() {
        int rawValue = sys->analogRead(pin);
        lastRawValue = rawValue;
        return rawValue;
    }

    bool isMiddle(int rawValue) {
        return rawValue >= this->zeroMin && rawValue <= this->zeroMax;
    }

    float readNorm() {
        int one;
        int zero;
        int value = readRaw();
        int direct = reverse ? -1 : 1;

        if (value > this->zeroMax) {
            one = this->max;
            zero = this->zeroMax;
        } else if (value < this->zeroMin) {
            one = this->min;
            zero = this->zeroMin;
            direct = -direct;
        } else {
            return 0.0f;
        }
        return (float)(value - zero) * direct / (float)(one - zero);
    }
};
} // namespace a8::util