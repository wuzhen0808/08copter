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

    // TODO delete this function.
    template <typename T>
    static T parse(const String &fStr) {
        T fV = .0f;
        parseFloat<T>(fStr, fV, 0);
        return fV;
    }

    template <typename T>
    static int parseFloat(const String &fStr, T &fValue, String *debug) {

        if (fStr.isEmpty()) {
            return -1;
        }
        int dotIdx = fStr.indexOf('.');
        int exp = dotIdx;
        if (dotIdx < 0) {
            exp = fStr.length();
        }
        exp -= 1;

        T fv = 0.0f;
        bool error = false;

        for (int i = 0; i < fStr.length(); i++) {
            if (i == dotIdx) {
                continue;
            }

            char ch = fStr.charAt(i);

            short sCh = (short)ch - (short)'0';
            if (sCh < 0 || sCh > 9) {
                error = true;
                sCh = 0;
            }
            if (debug != 0) {
                *debug << "i:" << i << ",sCh:" << sCh << ",fv:" << fv;
            }
            fv += (T)sCh * Math::power<T>(10, exp);
            if (debug != 0) {
                *debug << "=>" << fv;
                *debug << "\n";
            }
            exp--;
        }
        if (error) {
            return -1;
        }
        fValue = fv;
        return 1;
    }
};

} // namespace a8::util
