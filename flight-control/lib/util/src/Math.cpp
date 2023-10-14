#include "a8/util/Math.h"
#include <math.h>

namespace a8::util {

float Math::maxFloat() {
    return std::numeric_limits<float>::max();
}

long Math::map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Math::frexp_(float f, int *e) { return std::frexp(f, e); }
float Math::ldexp_(float f, int e) { return std::ldexp(f, e); }
float Math::trunc_(float f) { return std::trunc(f); }
double Math::frexp_(double f, int *e) { return std::frexp(f, e); }
double Math::ldexp_(double f, int e) { return std::ldexp(f, e); }
double Math::trunc_(double f) { return std::trunc(f); }

} // namespace a8::util