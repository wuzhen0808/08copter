#pragma once
#include "a8/ts/Joystick.h"
#include "a8/util.h"
namespace a8::ts {
using namespace a8::util;

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
        this->detectZero();
        this->detectOne(this->left, "LEFT");
        this->detectOne(this->right, "RIGHT");
    }

    void detectOne(Joystick *js, String side) {

        while (true) {
            js->waitMoving(String() << "Please MOVE the joystick of " << side << " side around.");

            long started = sys->getSteadyTime();
            long steadyInterval = 3000;

            long lastModified11 = started;
            long lastModified12 = started;

            int ret11 = -1;
            int ret12 = -1;

            bool retry = false;

            while (ret11 < 0 || ret12 < 0) {

                if (ret11 == -2 || ret12 == -2) {
                    retry = true;
                    break;
                }

                js->detectOne(started, lastModified11, lastModified12, steadyInterval, ret11, ret12);
            }
            if (retry) {
                continue;
            }
            
            break;
        }

        log("done of study the min/MAX scope of the joystick of " << side << " side.");
    }

    void detectZero() {

        long started = sys->getSteadyTime();
        long steadyInterval = 3000;

        long lastModified11 = started;
        long lastModified12 = started;
        long lastModified21 = started;
        long lastModified22 = started;

        int ret11 = -1;
        int ret12 = -1;
        int ret21 = -1;
        int ret22 = -1;

        log("going to study the ZERO area of joysticks, do NOT touch the joy sticks, both left and right.");

        while (true) {
            bool hasError = false;
            if (ret11 < 0 || ret12 < 0) {
                this->left->detectZero(started, lastModified11, lastModified12, steadyInterval, ret11, ret12);
                hasError = true;
            }
            if (ret21 < 0 || ret22 < 0) {
                this->right->detectZero(started, lastModified21, lastModified22, steadyInterval, ret21, ret22);
                hasError = true;
            }
            if (!hasError) {
                break;
            }
        }
        log("done of study zero area of joysticks.");
    }
};
} // namespace a8::ts