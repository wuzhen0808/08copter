#pragma once
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/native/SimInSkeleton.h"
#include "a8/util/Vector3f.h"


namespace a8::fc::native {
using namespace a8::fc;
using namespace a8::util;
class NativeAttitudeSensor : public AttitudeSensor {
private:
    SimInSkeleton *sis = 0;

public:
    NativeAttitudeSensor() : AttitudeSensor() {
    }
    ~NativeAttitudeSensor() {
    }

    void setSkeleton(SimInSkeleton *skeleton) {
        this->sis = skeleton;
    }

    int isReady(Result &rst) override {
        if (this->sis == 0) {
            rst.errorMessage << "waiting the simulator connect in.";
            return -1;
        }
        return 1;
    }

    /**
     * Altitude in Meter.
     */
    int getAltitude(double &altitude, Result &rst) {
        int ret = this->isReady(rst);
        if (ret < 0) {
            return ret;
        }
        return sis->getAltitude(altitude, rst);
    }
    int getLongitude(double &longitude, Result &rst) {
        int ret = this->isReady(rst);
        if (ret < 0) {
            return ret;
        }
        return sis->getLongitude(longitude, rst);
    }
    /**
     * Angular velocity in Degree.
     */
    int getAngVel(Vector3f &angVel, Result &rst) override {
        int ret = this->isReady(rst);
        if (ret < 0) {
            return ret;
        }
        return sis->getAngVel(angVel, rst);
    }
    /**
     * Acceleration velocity in ?
     */
    int getAccVel(Vector3f &accVel, Result &rst) override {
        int ret = this->isReady(rst);
        if (ret < 0) {
            return ret;
        }
        return sis->getAccVel(accVel, rst);
    }
};
} // namespace a8::fc::native