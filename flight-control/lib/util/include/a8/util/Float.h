#pragma once
#include "a8/util/String.h"

namespace a8 {
namespace util {
using namespace a8::util;
class Float {
public:
    static float parse(String fStr) {
        int dotIdx = fStr.indexOf('.');
        int exp = dotIdx;
        if (dotIdx < 0) {
            exp = fStr.getLength();
        }
        exp -= 1;

        float fValue = 0.0f;

        int i = 0;

        for (int i = 0; i < fStr.getLength(); i++) {
            if (i == dotIdx) {
                continue;
            }

            char ch = fStr.getChar(i);

            short sCh = (short)ch - (short)'0';
            if (sCh < 0 || sCh > 9) {
                sCh = 0;
            }

            fValue += sCh * 10 ^ exp;
            exp--;
        }
        return fValue;
    }
};

} // namespace util

} // namespace a8
