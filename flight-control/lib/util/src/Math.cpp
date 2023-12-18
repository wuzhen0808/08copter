#include "a8/util/Math.h"
#include <math.h>

namespace a8::util {
float Math::log10(float f) { return std::log10(f); };
float Math::pow10(float f) { return std::pow(10, f); };
double Math::log10(double f) { return std::log10(f); };
double Math::pow10(double f) { return std::pow(10, f); };

float Math::frexp_(float f, int *e) { return std::frexp(f, e); }
float Math::ldexp_(float f, int e) { return std::ldexp(f, e); }
float Math::trunc_(float f) { return std::trunc(f); }
double Math::frexp_(double f, int *e) { return std::frexp(f, e); }
double Math::ldexp_(double f, int e) { return std::ldexp(f, e); }
double Math::trunc_(double f) { return std::trunc(f); }
bool Math::isnan(float fValue){return std::isnan(fValue);}
bool Math::isnan(double fValue){return std::isnan(fValue);}
bool Math::isinf(float fValue){return std::isinf(fValue);}
bool Math::isinf(double fValue){return std::isinf(fValue);}

} // namespace a8::util