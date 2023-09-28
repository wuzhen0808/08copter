#pragma once

#define M_PI (3.141592653589793238462643383279502884)
#define FEET_TO_METERS (0.3048)
#define RAD_TO_DEG (180.0f / M_PI)
#define ZERO_NEAR (0.000001)

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

    static bool isNear(double fValue1, double fValue2) {
        float diff = fValue1 - fValue2;
        return -ZERO_NEAR < diff && diff < ZERO_NEAR;
    }
    static double power(double d1, int exp) {
        double rt = 1;
        if (exp > 0) {
            for (int i = 0; i < exp; i++) {
                rt = rt * d1;
            }
        } else {
            for (int i = 0; i > exp; i--) {
                rt = rt / d1;
            }
        }
        return rt;
    }
};
} // namespace a8::util