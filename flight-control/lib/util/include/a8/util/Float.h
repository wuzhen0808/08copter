#pragma once
#include "a8/util/Math.h"
#include "a8/util/String.h"
#include "a8/util/StringUtil.h"
namespace a8::util {

class Float {
public:
    template <typename T>
    static bool isSame(T d1, T d2, int precision) {
        int e1;
        T m1 = Math::frexp<T>(d1, &e1);
        long l1 = (long)Math::ldexp<T>(m1, precision);

        int e2;
        T m2 = Math::frexp<T>(d2, &e2);
        long l2 = (long)Math::ldexp<T>(m2, precision);

        return e1 == e2 && l1 == l2;
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

} // namespace a8::util
