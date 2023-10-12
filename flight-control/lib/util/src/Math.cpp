#include "a8/util/Math.h"
#include <math.h>

namespace a8::util {

float Math::maxFloat() {
    return std::numeric_limits<float>::max();
}

long Math::map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Math::frexp(float m, int *e) { return std::frexp(m, e); }
float Math::trunc(float f) { return std::trunc(f); }
float Math::ldexp(float m, int exp) { return std::ldexp(m, exp); }
double Math::frexp(double m, int *e) { return std::frexp(m, e); }
double Math::trunc(double f) { return std::trunc(f); }
double Math::ldexp(double m, int exp) { return std::ldexp(m, exp); }

} // namespace a8::util