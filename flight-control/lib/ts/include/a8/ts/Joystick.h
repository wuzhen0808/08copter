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

    void waitMoving(String prompt) {
        while (true) {
            log(prompt);
            int moving = waitMoving(5000);
            if (moving > 0) {
                log("start moving.");
                break;
            }
        }
    }
    int waitMoving(long timeout) {
        long started = sys->getSteadyTime();
        while (true) {
            if (xPin->isMoving()) {
                break;
            }
            if (yPin->isMoving()) {
                break;
            }

            if (sys->getSteadyTime() - started > timeout) {
                return -1;
            }
        }
        return 1;
    }

    void detectZero(long started, long &lastModified1, long &lastModified2, long steadyInterval, int &ret1, int &ret2) {
        if (ret1 < 0) {
            ret1 = this->xPin->detectZero(started, lastModified1, steadyInterval);
        }
        if (ret2 < 0) {
            ret2 = this->yPin->detectZero(started, lastModified2, steadyInterval);
        }
    }

    void detectOne(long started, long &lastModified1, long &lastModified2, long steadyInterval, int &ret1, int &ret2) {

        if (ret1 < 0) {
            ret1 = this->xPin->detectOne(started, lastModified1, steadyInterval);
        }
        if (ret2 < 0) {
            ret2 = this->yPin->detectOne(started, lastModified2, steadyInterval);
        }
    }
};
} // namespace a8::ts