#pragma once
#include "a8/util.h"
namespace a8::ts {
using namespace a8::util;

class Joystick : FlyWeight {
    AnalogInputPin *xPin;
    AnalogInputPin *yPin;
    System *sys;

public:
    Joystick(int xPin, int yPin, int zPin, System *sys, LoggerFactory *logFac) : FlyWeight(logFac, String() << "Joystick[" << xPin << "," << yPin << "," << zPin << "]") {
        this->sys = sys;
        this->xPin = new AnalogInputPin(xPin, sys, logFac);
        this->yPin = new AnalogInputPin(yPin, sys, logFac);
    }
    ~Joystick(){
        delete this->xPin;
        delete this->yPin;
    }
    AnalogInputPin *getXPin() {
        return xPin;
    }

    AnalogInputPin *getYPin() {
        return yPin;
    }

    Point2<int> getRaw() {
        int x = xPin->readRaw();
        int y = yPin->readRaw();
        return Point2<int>(x, y);
    }

    Point2<int> getZeroMin() {
        int x = xPin->getZeroMin();
        int y = yPin->getZeroMin();
        return Point2<int>(x, y);
    }

    Point2<int> getZeroMax() {
        int x = xPin->getZeroMin();
        int y = yPin->getZeroMin();
        return Point2<int>(x, y);
    }

};
} // namespace a8::ts