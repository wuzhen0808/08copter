#include "a8/util.h"
namespace a8::util {
using namespace a8::util;

class DigitalInputPin : FlyWeight {
private:
    System *sys;
    int pin;

public:
    DigitalInputPin(int pin, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, String() << "DigitalInputPin[" << pin << "]") {
        this->sys = sys;
        this->pin = pin;
        sys->setPinMode(pin, PinMode::IN);
    }

    bool isLow() {
        for (int i = 0; i < 10; i++) {

            int aValue = sys->analogRead(this->pin);
            if (aValue == 0) {
                return true;
            }
            sys->delay(1);
        }

        return false;
    }
    bool isHigh(){
        return !isLow();
    }

    int waitHighMs(long timeoutMs) {
        TimeUs started = sys->getSteadyTimeUs();
        while (true) {
            bool high = this->isHigh();
            if (high) {
                log("become high.");
                break;
            }
            if (sys->getSteadyTimeUs() - started > timeoutMs * 1000) {
                return -1;
            }
            this->sys->delay(1);
        }
        return 1;
    }
};
} // namespace a8::util