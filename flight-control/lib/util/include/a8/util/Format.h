#pragma once
#include "a8/util/Lang.h"
#include "a8/util/Math.h"

namespace a8::util {

class Format {
private:
    static bool set(char *buf, int idx, char ch, int bufLen) {
        if (idx >= 0 && idx < bufLen) {
            buf[idx] = ch;
            return true;
        }
        return false;
    }

public:
    template <typename T>
    static int format(char *buf, int bufLen, T value, int precision, bool addTail) {

        if (precision == 0) {
            return formatAsInt<T>(buf, bufLen, value, addTail);
        }
        T aValue = value < 0 ? -value : value;
        int exp = int(Math::log10<T>(aValue));
        float m = aValue / Math::pow10<T>(float(exp));
        if (m >= 1.0f) {
            m = m / 10;
            exp++;
        }

        int idx = 0;
        if (value < 0) {
            set(buf, idx++, '-', bufLen);
        }

        set(buf, idx++, '0', bufLen);
        set(buf, idx++, '.', bufLen);

        long longM = long(m * Math::pow10<T>(float(precision)));
        int mRightLen = formatAsInt<T>(buf + idx, bufLen - idx, longM, false);
        idx += mRightLen;
        set(buf, idx++, 'e', bufLen);

        if (exp < 0) {
            exp = -exp;
            set(buf, idx++, '-', bufLen);
        }

        int eRightLen = formatAsInt<T>(buf + idx, bufLen - idx, exp, false);
        idx += eRightLen;
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
        if (rValue < 0) {
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
    static void append(char *&bufRef, int &lenRef, int &capRef, int deltaCap, //
                       int preferWidth, char fillLeading, T value, int precision, bool addTail) {

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
        int lenRight = format<T>(bufRight, capRight, value, precision, false);
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
            lenRight = format<T>(bufRight + leading, capRight - leading, value, precision, false);

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