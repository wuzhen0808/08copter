#pragma once
#include "a8/fc/AttitudeSensor.h"
#include "a8/fc/FlightControl.h"
#include "a8/fc/native/SimInSkeleton.h"
#include "a8/util/Vector3f.h"

using namespace a8::fc;
using namespace a8::util;
using namespace a8::hal::native;

namespace a8::fc::native {
class NativeAttitudeSensor : public AttitudeSensor {
private:
    SimInSkeleton *sis;

public:
    NativeAttitudeSensor(SimInSkeleton *sis) : AttitudeSensor() {
        this->sis = sis;
    }
    ~NativeAttitudeSensor() {
    }
    /**
     * Altitude in Meter.
     */
    int getAltitude(double &altitude, Result &rst) {
        return sis->getAltitude(altitude, rst);
    }
    int getLongitude(double &longitude, Result &rst) {
        return sis->getLongitude(longitude, rst);
    }
    /**
     * Angular velocity in Degree.
     */
    int getAngVel(Vector3f &angVel, Result &rst) override {
        return sis->getAngVel(angVel, rst);
    }
    /**
     * Acceleration velocity in ?
     */
    int getAccVel(Vector3f &accVel, Result &rst) override {
        return sis->getAccVel(accVel, rst);
    }
};
} // namespace a8::fc::native