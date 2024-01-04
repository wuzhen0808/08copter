#pragma once
#include "a8/util.h"
namespace a8::fc {

using namespace a8::util;

enum UnBalanceAction {
    ASK = 0,
    IGNORE = 1,
    IGNORE_IF_SAFE = 2,
    END_OF_MISSION = 3
};
enum class ImuFilter : int {
    NONE = 0,
    MAG = 1,
    MOH = 2
};

extern Options<ImuFilter> IMU_FILTERS;
int operator%(ImuFilter &filter, int iValue);
String &operator<<(String &str, const ImuFilter f);

enum RpyFilter : int {
    NO = 0,
    LP1 = 1,
    LP2 = 2
};

extern Options<RpyFilter> RPY_FILTERS;
int operator%(RpyFilter &filter, int iValue);
String &operator<<(String &str, const RpyFilter f);

extern EnumType<UnBalanceAction> UN_BALANCE_ACTIONS;
String &operator<<(String &str, const UnBalanceAction f);
void initialize();

} // namespace a8::fc
