#pragma once
#include "a8/util/Array.h"
#include "a8/util/Debug.h"
#include "a8/util/Lang.h"
#include "a8/util/Math.h"

#ifndef A8_FORMAT_DEBUG
#define A8_FORMAT_DEBUG (0)
#endif

/**
 * Know issue:
 * If precision large than 9, the double 1000 will be formatted to '2147', 2000 is to '4294';
 * so we limit it to 8;
 */
#define A8_MAX_PRECISION (8)

namespace a8::util {

class Format {
public:
    class Float {
    public:
        static void get(int exp, const Float *this_, int &precision, int &pointOffset, int &tailPrecision) {
            if (this_ == 0) {
                precision = 6;
                tailPrecision = 3;
                int aExp = Math::abs(exp);
                if (aExp > precision) {
                    pointOffset = 1;
                } else {
                    pointOffset = exp;
                }
                return;
            }
            this_->get(exp, precision, pointOffset, tailPrecision);
        }

        virtual void get(int exp, int &precision, int &pointOffset, int &tailPrecision) const = 0;
    };

    class AutoOffsetFloat : public Float {
        int precision = 6;
        int tailPrecision = 3;

    public:
        AutoOffsetFloat(const int precision, const int tailPrecision) {
            this->precision = precision;
            this->tailPrecision = tailPrecision;
            if (this->precision > A8_MAX_PRECISION) {
                // avoid overflow.
                // TODO:
                // According to the type of the float, overflow boundary should be clear defined.
                //
                this->precision = A8_MAX_PRECISION;
            }

            if (this->tailPrecision > this->precision) {
                this->tailPrecision = this->precision;
            }
        }
        void get(int exp, int &precision, int &pointOffset, int &tailPrecision) const override {
            precision = this->precision;
            tailPrecision = this->tailPrecision;
            int aExp = Math::abs(exp);
            if (aExp > this->precision) {
                pointOffset = 1;
            } else {
                pointOffset = exp;
            }
        }
    };

private:
    // set may failed if no space for the buf to storage the ch..

    static bool copy(char *buf, int idx2, int idx1, int bufLen) {
        if (idx1 < bufLen) {
            return set(buf, idx2, buf[idx1], bufLen);
        }
        return false;
    }
    static void shift(char *buf, int fromIdx, int len, int shift, int bufLen) {
        if (shift == 0) {
            // do nothing.
        } else if (shift < 0) {
            // shift left
            for (int i = 0; i < len; i++) {
                char ch;
                if (get(buf, fromIdx + i, ch, bufLen)) {
                    set(buf, fromIdx + i + shift, ch, bufLen);
                }
            }
        } else {
            // shift right
            for (int i = 0; i < len; i++) {
                char ch;
                if (get(buf, fromIdx + len - 1 - i, ch, bufLen)) {
                    set(buf, fromIdx + len - 1 - i + shift, ch, bufLen);
                }
            }
        }
    }
    static int write(char *buf, int idx, const char *ch, int bufLen) {
        int i = 0;
        while (true) {
            if (ch[i] == '\0') {
                break;
            }
            set(buf, idx + i, ch[i], bufLen);
            i++;
        }

        return i;
    }
    static bool get(char *buf, int idx, char &ch, int bufLen) {
        if (idx >= 0 && idx < bufLen) {
            ch = buf[idx];
            return true;
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

    template <typename T>
    static int formatNumber(char *buf, int bufLen, T value, bool asFloat, const Format::Float *format, bool addEndOfStr) {

        if (asFloat) {
            return formatAsFloat<T>(buf, bufLen, value, format, addEndOfStr);
        } else {
            return formatAsInt<T>(buf, bufLen, value, addEndOfStr);
        }
    }

    template <typename T>
    static void doAppendNumber(char *&bufRef, int &lenRef, int &capRef, int deltaCap, //
                               int preferWidth, char fillLeading, T value, bool asFloat, const Format::Float *format, bool addEndOfStr) {

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
        int lenRight = formatNumber<T>(bufRight, capRight, value, asFloat, format, false);
        int lenTail = lenRight; // the content
        int leading = 0;
        if (lenRight < preferWidth) {
            leading = preferWidth - lenRight;
            lenRight = preferWidth;
        }

        if (lenRight + 1 > capRight) { // str truncate,

            capacity = Array::makeCapacity(capacity, deltaCap, lenLeft + lenRight + 1);
            newBuf = new char[capacity];
            Array::copy<char>(buf, 0, lenLeft, newBuf);
            delete[] buf;
            buf = newBuf;

            capRight = capacity - lenLeft;
            bufRight = buf + lenLeft;
            // format again, with a larger capacity.
            lenRight = formatNumber<T>(bufRight + leading, capRight - leading, value, asFloat, format, false);

            if (lenRight + 1 > capRight) {
                // error processing?
                // exit(1);
            }
        } else { // move for leading space
            Array::shift<char>(bufRight, lenTail, leading);
        }
        if (newBuf != 0) {
            bufRef = newBuf;   // write back new buf;
            capRef = capacity; // write back new capacity.
        }
        // leading fill
        Lang::fill<char>(bufRight, leading, fillLeading);
        lenRef = lenLeft + lenRight; // write back new length.
        if (addEndOfStr) {
            buf[lenRef] = '\0';
        }
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

    static int formatAsFloat(char *buf, int bufLen, bool neg, float m, int exp, const Format::Float *format, bool addEndOfStr) {

#if A8_FORMAT_DEBUG == 1
        A8_DEBUG6("formatAsFloat,", neg, m, exp, precision, pointOffset);
#endif
        int precision, pointOffset, tailPrecision;
        Format::Float::get(exp, format, precision, pointOffset, tailPrecision);
        int idx = 0;

        if (neg) {
            set(buf, idx++, '-', bufLen);
        }
        long power = Math::pow10<long>((long)precision);
        long number = long(m * power);
        int pointIdx = -1;
        if (pointOffset <= 0) {
            // '0.00000'
            // if pointOffset <= 0, then moving number right, insert zero before number and after point.
            set(buf, idx++, '0', bufLen);
            pointIdx = idx;
            set(buf, idx++, '.', bufLen);
            for (int i = 0; i > pointOffset; i--) {
                set(buf, idx++, '0', bufLen);
            }
        } else { // pointOffset > 0 : no zero before point.
            // number, numberLen should = precision.
            pointIdx = idx + pointOffset; // remember the point position for positive pointOffset.
        }

        int numberLen = formatAsInt<long>(buf + idx, bufLen - idx, number, false);
        // numberLen == precision, what if not equal?
        idx += numberLen;

        // find a position for point if offset > 0.
        if (pointOffset > 0) {

            int tailNumberLen = numberLen - pointOffset;
            if (tailNumberLen > 0) { // shift tail number right & reserve one more space for the point position.
                shift(buf, pointIdx, tailNumberLen, 1, bufLen);
                idx++;
                // got the point space and set it.
                set(buf, pointIdx, '.', bufLen);

            } else {
                // append 0 if number tail is negative.
                for (int i = 0; i > tailNumberLen; i--) {
                    set(buf, idx++, '0', bufLen);
                }
                // set the point at the end and append 0 to show it is a float value, not int.
                set(buf, pointIdx, '.', bufLen);
                set(buf, pointIdx, '0', bufLen);
            }
        }
        // process tail precision,

        if (tailPrecision == 0) {
            idx = pointIdx;
        } else if (tailPrecision > 0) {
            int idx2 = pointIdx + 1 + tailPrecision;
            for (int i = idx; i < idx2; i++) { // append 0 if need.
                set(buf, i, '0', bufLen);
            }
            idx = idx2;
        } else {
            // let it be.
        }
        //

        int expTail = exp - pointOffset;
        // exp-part.
        if (expTail != 0) {
            set(buf, idx++, 'e', bufLen);
            idx += formatAsInt<int>(buf + idx, bufLen - idx, expTail, false);
        }
        if (addEndOfStr && idx < bufLen) {
            buf[idx] = '\0';
        }
        return idx;
    }

    template <typename T>
    static int resolveFloat(float value, bool &neg, float &m, int &exp) {
        if (Math::isnan<T>(value)) {
            return -1;
        }
        if (Math::isinf<T>(value)) {
            return -2;
        }
        if (Math::isZero<T>(value)) {
            m = 0.0f;
            neg = false;
            exp = 0;
            return 0;
        }
        neg = value < -0.0f;
        T aValue = neg ? (-value) : value; // convert to positive.
        T fExp = Math::log10<T>(aValue);
        exp = int(fExp);

        // float m = 123456;
        m = aValue / Math::pow10<T>(float(exp));
        if (m >= 1.0f) {
            m = m / 10;
            exp++;
        }
        return 1;
    }

    template <typename T>
    static int formatAsFloat(char *buf, int bufLen, T value, const Format::Float *format, bool addEndOfStr) {
#if A8_FORMAT_DEBUG == 1
        A8_DEBUG4("formatAsFloat,", value, precision, pointOffset);
#endif
        bool neg;
        float m;
        int exp;

        int ret = resolveFloat<T>(value, neg, m, exp);

        if (ret < 0) {
            if (ret == -1) {
                return write(buf, 0, "nan", bufLen);
            }
            if (ret == -2) {
                return write(buf, 0, "inf", bufLen);
            }
            return write(buf, 0, "?", bufLen);
        }

        return formatAsFloat(buf, bufLen, neg, m, exp, format, addEndOfStr);
    }

    template <typename T>
    static int formatAsInt(char *buf, int bufLen, T iValue, bool addEndOfStr) {
        T rValue = iValue;
        int idx = bufLen - 1;
        bool neg = false;
        if (rValue < -0.0f) {
            rValue = -rValue;
            neg = true;
        }
        for (;;) {
            // process reversely, from right to left.
            // at least print one zero.
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

        // shift the result to the most left of the buffer if has space.
        if (len < bufLen) {
            int space = bufLen - len;
            Array::shift(buf + space, len, -space);
        } else {
            // buffer overflow, result is truncated, need shift to right?
            //
        }

        if (addEndOfStr) {
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
    static void appendNumberAsInt(char *&bufRef, int &lenRef, int &capRef, int deltaCap, //
                                  int preferWidth, char fillLeading, T value, bool addEndOfStr) {
        doAppendNumber(bufRef, lenRef, capRef, deltaCap, preferWidth, fillLeading, value, false, 0, addEndOfStr);
    }

    template <typename T>
    static void appendNumberAsFloat(char *&bufRef, int &lenRef, int &capRef, int deltaCap, //
                                    int preferWidth, char fillLeading, T value, const Format::Float *format, bool addEndOfStr) {
        doAppendNumber(bufRef, lenRef, capRef, deltaCap, preferWidth, fillLeading, value, true, format, addEndOfStr);
    }
};

} // namespace a8::util
