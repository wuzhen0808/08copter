#pragma once
#include "a8/util/Lang.h"
#include "a8/util/Math.h"
#define A8_FORMAT_DEBUG (0)
namespace a8::util {

class Format {
private:
    // set may failed if no space for the buf to storage the ch..
    static bool copy(char *buf, int idx2, int idx1, int bufLen) {
        if (idx1 < bufLen) {
            return set(buf, idx2, buf[idx1], bufLen);
        }
        return false;
    }
    static bool set(char *buf, int idx, char ch, int bufLen) {
        if (idx >= 0 && idx < bufLen) {
            buf[idx] = ch;
            return true;
        }
        return false;
    }

public:
    /**
     * Return the length of the formatted string.
     * The length may great than the provided buffer len, in this case the string will be truncated.
     *
     */

    template <typename T>
    static bool isZero(T fValue) {
        return fValue > -1e-20 && fValue < 1e-20;
    }

    template <typename T>
    static int formatAsFloat(char *buf, int bufLen, T value, int precision, bool addTail) {
        if (precision < 1) {
            precision = 1; // precision at least is 1.
        }

        int idx = 0;
        T aValue = value < -0.0f ? (-value) : value;
        int exp = int(Math::log10<T>(aValue));
        float m = aValue / Math::pow10<T>(float(exp));
        if (m >= 1.0f) {
            m = m / 10;
            exp++;
        }

        if (value < 0.0f) {
            set(buf, idx++, '-', bufLen);
        }

        long longM = long(m * Math::pow10<T>(float(precision)));
        // point.
        set(buf, idx++, '.', bufLen);
        // m-part.
        int mLen = formatAsInt<long>(buf + idx, bufLen - idx, longM, false);
        idx += mLen;
        // exp-part.
        if (exp != 0) {
            set(buf, idx++, 'e', bufLen);
            // if (exp < 0) {
            //     exp = -exp;
            //     set(buf, idx++, '-', bufLen);
            // }

            int eLen = formatAsInt<int>(buf + idx, bufLen - idx, exp, false);
            idx += eLen;
        }
        if (addTail && idx < bufLen) {
            buf[idx] = '\0';
        }
        return idx;
    }

    template <typename T>
    static int formatAsInt(char *buf, int bufLen, T iValue, bool addTail) {
        T rValue = iValue;
        int idx = bufLen - 1;
        bool neg = false;
        if (rValue < -0.0f) {
            rValue = -rValue;
            neg = true;
        }
        for (;;) {
            int d = (int)Math::mod10<T>(rValue);
            set(buf, idx--, '0' + d, bufLen);
            rValue = rValue / 10;

            if ((int)rValue == 0) {
                break;
            }
        }
        if (neg) {
            set(buf, idx--, '-', bufLen);
        }
        int len = bufLen - 1 - idx;

        if (len < bufLen) {
            int space = bufLen - len;
            Lang::shift(buf + space, len, -space);
        }
        if (addTail) {
            set(buf, len, '\0', bufLen);
        }
        return len;
    }

    /**
     * Format and append the formatted string to the buffer.
     *
     * Write back the results by reference.
     *
     * The buffer must be a space created by new in heap or zero in which case a new one will be created.
     *
     */
    template <typename T>
    static int formatNumber(char *buf, int bufLen, T value, bool asFloat, int floatPrecision, bool addTail) {
        if (asFloat) {
            return formatAsFloat(buf, bufLen, value, floatPrecision, addTail);
        } else {
            return formatAsInt(buf, bufLen, value, addTail);
        }
    }

    template <typename T>
    static void appendNumber(char *&bufRef, int &lenRef, int &capRef, int deltaCap, //
                             int preferWidth, char fillLeading, T value, bool asFloat, int floatPrecision, bool addTail) {

        char *buf = bufRef;
        int capacity = capRef;
        int lenLeft = lenRef;
        char *newBuf = 0;
        if (buf == 0) {
            // assume capacity == lenLeft == 0;
            capacity = deltaCap;
            buf = newBuf = new char[capacity]; // init with a capacity to try format.
        }

        int capRight = capacity - lenLeft; // remaining capacity
        char *bufRight = buf + lenLeft;    // give a offset to storage result.
        // try format first time, and then check the length.
        int lenRight = formatNumber<T>(bufRight, capRight, value, asFloat, floatPrecision, false);
        int lenTail = lenRight; // the content
        int leading = 0;
        if (lenRight < preferWidth) {
            leading = preferWidth - lenRight;
            lenRight = preferWidth;
        }

        if (lenRight + 1 > capRight) { // str truncate,

            capacity = Lang::makeCapacity(capacity, deltaCap, lenLeft + lenRight + 1);
            newBuf = new char[capacity];
            Lang::copy<char>(buf, 0, lenLeft, newBuf);
            delete[] buf;
            buf = newBuf;

            capRight = capacity - lenLeft;
            bufRight = buf + lenLeft;
            // format again, with a larger capacity.
            lenRight = formatNumber<T>(bufRight + leading, capRight - leading, value, asFloat, floatPrecision, false);

            if (lenRight + 1 > capRight) {
                // error processing?
                // exit(1);
            }
        } else { // move for leading space
            Lang::shift<char>(bufRight, lenTail, leading);
        }
        if (newBuf != 0) {
            bufRef = newBuf;   // write back new buf;
            capRef = capacity; // write back new capacity.
        }
        // leading fill
        Lang::fill<char>(bufRight, leading, fillLeading);
        lenRef = lenLeft + lenRight; // write back new length.
        if (addTail) {
            buf[lenRef] = '\0';
        }
    }
};

} // namespace a8::util
