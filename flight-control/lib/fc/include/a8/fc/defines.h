#pragma once
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
enum class ImuFilter : int {
    NONE = 0,
    MAG = 1,
    MOH = 2
};
extern Options<ImuFilter> IMU_FILTERS;
int operator%(ImuFilter &filter, int iValue);
String &operator<<(String &str, const ImuFilter f);
void initialize();

} // namespace a8::fc
