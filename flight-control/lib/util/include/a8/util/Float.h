#pragma once
#include "a8/util/Math.h"
#include "a8/util/String.h"
#include "a8/util/StringUtil.h"
namespace a8 {
namespace util {
using namespace a8::util;
class Float {
public:
    static bool isNear(double d1, double d2) {
        return Math::isNear(d1, d2);
    }
    
    template <typename T>
    static T parse(const String &fStr) {
        int dotIdx = fStr.indexOf('.');        
        int exp = dotIdx;
        if (dotIdx < 0) {
            exp = fStr.length();
        }
        exp -= 1;

        T fValue = 0.0f;

        int i = 0;

        for (int i = 0; i < fStr.length(); i++) {
            if (i == dotIdx) {
                continue;
            }

            char ch = fStr.charAt(i);

            short sCh = (short)ch - (short)'0';
            if (sCh < 0 || sCh > 9) {
                sCh = 0;
            }

            fValue += (T)sCh * Math::power(10, exp);
            exp--;
        }
        return fValue;
    }
};

} // namespace util

} // namespace a8
