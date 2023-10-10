#pragma once
#define M_PI_ (3.14159265358979323846)
#define FEET_TO_METERS (0.3048)
#define RAD_TO_DEG (180.0f / M_PI_)
#define ZERO_NEAR (0.000001)
namespace a8::util {

class Math {
public:
    template <typename T>
    static T min(T t1, T t2) {
        return t1 < t2 ? t1 : t2;
    }

    static long map(long x, long in_min, long in_max, long out_min, long out_max);
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

    static float maxFloat();

    static double frexp(double m, int *e);

    static float frexpf(float m, int *e);

    static float trunc(float f);

    static double trunc(double f);

    static double ldexp(long m, int exp);
};
} // namespace a8::util