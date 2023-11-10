#pragma once
#define M_PI_ (3.14159265358979323846)
#define FEET_TO_METERS (0.3048)
#define M_RAD_TO_DEG (180.0f / M_PI_)
#define ZERO_NEAR (0.000001)
namespace a8::util {

class Math {
private:
    static float frexp_(float f, int *e);
    static float ldexp_(float f, int e);
    static float trunc_(float f);
    static double frexp_(double f, int *e);
    static double ldexp_(double f, int e);
    static double trunc_(double f);

    static float log10(float f);
    static double log10(double f);
    static float pow10(float f);
    static double pow10(double f);

public:
    template <typename T>
    static T mod10(T value) {
        if (sizeof(T) == sizeof(int)) {
            return int(value) % 10;
        } else {
            return long(value) % 10;
        }
    }
    template <typename T>
    static T log10(T value) {
        if (sizeof(T) == sizeof(float)) {
            return log10((float)value);
        } else {
            return log10((double)value);
        }
    }

    template <typename T>
    static T pow10(T value) {
        if (sizeof(T) == sizeof(float)) {
            return pow10((float)value);
        } else {
            return pow10((double)value);
        }
    }

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
        return radian * M_RAD_TO_DEG;
    }

    template <typename T>
    static T power(T d1, int exp) {
        T rt = 1;
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

    template <typename F>
    static F frexp(F m, int *e) {
        if (sizeof(F) < 33) {
            return frexp_((float)m, e);
        } else {
            return frexp_((double)m, e);
        }
    }

    template <typename F>
    static F trunc(F f) {
        if (sizeof(F) < 33) {
            return trunc_((float)f);
        } else {
            return trunc_((double)f);
        }
    }

    template <typename F>
    static F ldexp(F m, int exp) {
        if (sizeof(F) < 33) {
            return ldexp_((float)m, exp);
        } else {
            return ldexp_((double)m, exp);
        }
    }
};
} // namespace a8::util