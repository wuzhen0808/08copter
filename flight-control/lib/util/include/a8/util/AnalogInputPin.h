#include "a8/util/FlyWeight.h"
#include "a8/util/System.h"
namespace a8::util {

class AnalogInputPin : public FlyWeight {
private:
    System *sys;
    int rawValueLeftMost = -1;
    int rawValueZeroLeft = -1;
    int rawValueZeroRight = -1;
    int rawValueRightMost = -1;
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
        int zeroWidth = this->rawValueZeroRight - this->rawValueZeroLeft + 1;

        if (raw < this->rawValueZeroLeft - zeroWidth * 2 || raw > this->rawValueZeroRight + zeroWidth * 2) {
            log("moving.");
            return true;
        }
        return false;
    }

    int waitMoving(long timeout) {
        long started = sys->getSteadyTime();
        while (true) {
            if (this->isMoving()) {
                break;
            }
            if (sys->getSteadyTime() - started > timeout) {
                return -1;
            }
        }
        return 1;
    }

    int detectOne(long started, long &lastModified, long steadyInterval) {

        int ret = this->resolveMinMax(this->rawValueLeftMost, this->rawValueRightMost, started, lastModified, steadyInterval);
        if (ret < 0) {
            return -1;
        }

        if (this->rawValueRightMost - this->rawValueLeftMost < 1000) {
            log(String() << "the norm area is too small:" << this->rawValueLeftMost << "/" << this->rawValueRightMost << ",retrying.");

            return -2;
        } else if (this->rawValueRightMost < this->rawValueZeroRight || this->rawValueLeftMost > this->rawValueZeroLeft) {
            log(String() << "the norm area is not fully cover the zero area:" << this->rawValueLeftMost << "/"
                         << this->rawValueZeroLeft << "/" << this->rawValueZeroRight << "/" << this->rawValueRightMost << ",retrying.");
            return -3;
        }

        log(String() << "got min/max value:" << this->rawValueLeftMost << "/" << this->rawValueRightMost);
        return 1;
    }

    int detectZero(long started, long &lastModified, long steadyInterval) {

        int ret = resolveMinMax(this->rawValueZeroLeft, this->rawValueZeroRight, started, lastModified, steadyInterval);
        if (ret < 0) {
            return ret;
        }
        if (ret > 0 && this->rawValueZeroRight - this->rawValueZeroLeft > 200) {

            log(String() << "the zero area is too large:" << this->rawValueZeroLeft << "/" << this->rawValueZeroRight << ",retrying.");
            return -1;
        }

        log(String() << "got zero area:" << this->rawValueZeroLeft << "/" << this->rawValueZeroRight);
        return 1;
    }

    int resolveMinMax(int &min, int &max, long started, long &lastModified, long steadyInterval) {

        int rValue = readRaw();

        if (rValue < min || min == -1) {
            min = rValue;
            lastModified = sys->getSteadyTime();
        }
        if (rValue > max || max == -1) {
            max = rValue;
            lastModified = sys->getSteadyTime();
        }

        if (sys->getSteadyTime() - lastModified > steadyInterval) {
            return 1;
        } else {
            return -1;
        }
    }

    int getLastRawRead() {
        return this->lastRawValue;
    }

    int readRaw() {
        int rawValue = sys->analogRead(pin);
        lastRawValue = rawValue;
        return rawValue;
    }

    bool isMiddle(int rawValue) {
        return rawValue >= this->rawValueZeroLeft && rawValue <= this->rawValueZeroRight;
    }

    float readNorm() {
        int one;
        int zero;
        int value = readRaw();
        int direct = reverse ? -1 : 1;

        if (value > this->rawValueZeroRight) {
            one = this->rawValueRightMost;
            zero = this->rawValueZeroRight;
        } else if (value < this->rawValueZeroLeft) {
            one = this->rawValueLeftMost;
            zero = this->rawValueZeroLeft;
            direct = -direct;
        } else {
            return 0.0f;
        }
        return (float)(value - zero) * direct / (float)(one - zero);
    }
};
} // namespace a8::util