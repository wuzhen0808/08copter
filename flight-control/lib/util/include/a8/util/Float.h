#pragma once
#include "a8/util/Math.h"
#include "a8/util/String.h"
namespace a8 {
namespace util {
using namespace a8::util;
class Float {
public:
    static bool isNear(double d1, double d2) {
        return Math::isNear(d1, d2);
    }

    static Buffer<double> parseAll(String fStr, char sep) {
        return parseAll<double>(fStr, sep, [](double d) { return d; });
    }
    template <typename T>
    static Buffer<T> parseAll2(String fStr, char sep, T (*converter)(double)) {
        Buffer<T> ret;
        ret.append(converter(1));
        return ret;
    }
    template <typename T>
    static Buffer<T> parseAll(String fStr, char sep, T (*converter)(double)) {

        Buffer<String> buf = fStr.split(sep);
        Buffer<T> ret;
        for (int i = 0; i < buf.getLength(); i++) {
            String str = buf.get(i);
            T dV = parse(str, converter);
            ret.append(dV);
        }
        return ret;
    }

    static double parse(String fStr) {
        return parse<double>(fStr, [](double d) { return d; });
    }

    template <typename T>
    static T parse(String fStr, T (*converter)(double)) {
        int dotIdx = fStr.indexOf('.');
        int exp = dotIdx;
        if (dotIdx < 0) {
            exp = fStr.getLength();
        }
        exp -= 1;

        double fValue = 0.0f;

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

            fValue += (double)sCh * Math::power(10, exp);
            exp--;
        }
        return converter(fValue);
    }
};

} // namespace util

} // namespace a8
