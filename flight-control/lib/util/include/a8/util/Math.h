#pragma once

#define M_PI (3.141592653589793238462643383279502884)
#define FEET_TO_METERS (0.3048)
#define RAD_TO_DEG (180.0f / M_PI)

namespace a8::util {

class Math {
public:
    static long map(long x, long in_min, long in_max, long out_min, long out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    template <typename T>
    static T &feetToMeters(T &feet) {
        return feet * FEET_TO_METERS;
    }

    template <typename T>
    static T &radian2Degree(T &radian) {
        return radian * RAD_TO_DEG;
    }
};
} // namespace a8::util