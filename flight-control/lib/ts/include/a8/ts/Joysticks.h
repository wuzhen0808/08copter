#pragma once
#include "a8/ts/Joystick.h"
#include "a8/util.h"
namespace a8::ts {
using namespace a8::util;
class Axis {

protected:
    String name;

public:
    Axis(String name) {
        this->name = name;
    }

    bool operator==(const Axis &axis) const {
        return this->name == axis.name;
    }

    friend String &operator<<(String &str, const Axis &axis) {
        return str << axis.name;
    }
};

static const Axis X = Axis("X");
static const Axis Y = Axis("Y");

class Joysticks : FlyWeight {
    Joystick *left;
    Joystick *right;
    System *sys;

public:
    Joysticks(int x1Pin, int y1Pin, int z1Pin, int x2Pin, int y2Pin, int z2Pin, System *sys, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->sys = sys;
        this->left = new Joystick(x1Pin, y1Pin, z1Pin, sys, logFac);
        this->right = new Joystick(x2Pin, y2Pin, z2Pin, sys, logFac);
    }

    void setup() {
        this->detectZeroMinMax(this->left, X, "LEFT");
        this->detectZeroMinMax(this->left, Y, "LEFT");
        this->detectZeroMinMax(this->right, X, "RIGHT");
        this->detectZeroMinMax(this->right, Y, "RIGHT");

        this->detectMinMax(this->left, X, "LEFT");
        this->detectMinMax(this->left, Y, "LEFT");
        this->detectMinMax(this->right, X, "RIGHT");
        this->detectMinMax(this->right, Y, "RIGHT");
    }

    void waitMoving(AnalogInputPin *aPin, String prompt) {
        while (true) {
            log(prompt);
            int moving = waitMovingMs(aPin, 5000);
            if (moving > 0) {
                log("start moving.");
                break;
            }
        }
    }

    int waitMovingMs(AnalogInputPin *aPin, long timeoutMs) {
        long started = sys->getSteadyTimeUs();
        while (true) {
            if (aPin->isMoving()) {
                break;
            }

            if (sys->getSteadyTimeUs() - started > timeoutMs * 1000) {
                return -1;
            }
        }
        return 1;
    }
    void detectMinMax(AnalogInputPin *aPin, int &min, int &max) {
        TimeUs started = sys->getSteadyTimeUs();
        TimeUs steadyInterval = 3000000;

        TimeUs lastModified = started;

        while (true) {

            int rValue = aPin->readRaw();

            if (rValue < min || min == -1) {
                min = rValue;
                lastModified = sys->getSteadyTimeUs();
            }
            if (rValue > max || max == -1) {
                max = rValue;
                lastModified = sys->getSteadyTimeUs();
            }

            if (sys->getSteadyTimeUs() - lastModified > steadyInterval) {
                // timeout .
                break;
            }
        }
        return;
    }
    void detectMinMax(Joystick *js, Axis axis, String side) {
        AnalogInputPin *aPin = (X == axis ? js->getXPin() : js->getYPin());
        while (true) {
            this->waitMoving(aPin, String() << "Please MOVE " << axis << " axis of " << side << " side joystick.");

            bool retry = false;
            int min = -1;
            int max = -1;
            this->detectMinMax(aPin, min, max);
            // check the value.
            int zeroMax = aPin->getZeroMax();
            int zeroMin = aPin->getZeroMin();
            if (max - min < 1000) {
                log(String() << "the norm area is too small:" << min << "/" << max << ",retrying.");
                retry = true;
            }
            if (max < zeroMax || min > zeroMin) {
                log(String() << "the norm area is not fully cover the zero area:" << min << "/"
                             << zeroMin << "/" << zeroMax << "/" << max << ",retrying.");
                retry = true;
            }

            if (retry) {
                continue;
            }

            aPin->setMin(min);
            aPin->setMax(max);

            log(String() << "done of study the scope(" << min << "," << max << ") of the " << axis << " axis of the " << side << " side joystick.");
            break;
        }
    }

    void detectZeroMinMax(Joystick *js, Axis axis, String side) {
        AnalogInputPin *aPin = (X == axis ? js->getXPin() : js->getYPin());
        while (true) {

            bool retry = false;
            int zeroMin = -1;
            int zeroMax = -1;
            this->detectMinMax(aPin, zeroMin, zeroMax);
            // check the value.
            if (zeroMax - zeroMin > 200) {

                log(String() << "the zero area is too large:" << zeroMin << "/" << zeroMax << ",retrying.");
                retry = true;
            }

            if (retry) {
                continue;
            }

            aPin->setZeroMin(zeroMin);
            aPin->setZeroMax(zeroMax);

            log(String() << "done of study the zero scope(" << zeroMin << "," << zeroMax << ") of the " << axis << " axis of the " << side << " side joystick.");
            break;
        }
    }
};
} // namespace a8::ts