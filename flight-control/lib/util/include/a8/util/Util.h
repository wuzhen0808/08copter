#pragma once
#include <iostream>
namespace a8::util {

class Util {
public:
    static const char END_OF_STR = '\0';

    template <typename... Args>
    static void appendFormat(char *&bufRef, int &lenRef, int &capRef, int deltaCap, const char formatStr[], Args... args) {

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
        int lenRight = snprintf(bufRight, capRight, formatStr, args...);

        if (lenRight + 1 > capRight) { // str truncate,

            capacity = Util::makeCapacity(capacity, deltaCap, lenLeft + lenRight + 1);
            newBuf = new char[capacity];
            Util::copy<char>(buf, 0, lenLeft, newBuf);
            delete[] buf;
            buf = newBuf;

            capRight = capacity - lenLeft;
            bufRight = buf + lenLeft;
            lenRight = snprintf(bufRight, capRight, formatStr, args...);

            if (lenRight +1 > capRight) {
                // error processing?
                // exit(1);
            }
        }
        if (newBuf != 0) {
            bufRef = newBuf;   // write back new buf;
            capRef = capacity; // write back new capacity.
        }
        lenRef = lenLeft + lenRight; // write back new length.
    }

    template <typename T>
    static void reRef(T *&ref, T *newRef) {
        T *old = ref;
        ref = newRef;
        delete old;
    }

    static void bug() {
        std::cout << "bug" << std::endl;
    }

    static void appendStr(char *&bufferRef, int &lengthRef, int &capacityRef, int deltaCapacity,
                          const char *buf1, const int from1, const int len1) {
        int lenLeft = lengthRef;
        int lenRight = len1;
        append(bufferRef, lengthRef, capacityRef, deltaCapacity, 1, buf1, from1, len1);
        if (lengthRef != lenLeft + lenRight) {
            lengthRef = 0;
        }
        if (bufferRef != 0) {
            bufferRef[lengthRef] = END_OF_STR;
        }
    }

    template <typename T>
    static void append(T *&bufferRef, int &lengthRef, int &capacityRef, int deltaCapacity, int additionalCap,
                       const T *buf1, const int from1, const int len1) {

        int length3 = lengthRef + len1;

        int cap2 = capacityRef;
        bool new_ = false;
        while (cap2 < length3 + additionalCap) { // find a new capacity.
            cap2 += deltaCapacity;
            new_ = true;
        }
        if (new_) {
            T *buffer = newArray<T>(bufferRef, 0, lengthRef, 0, 0, 0, cap2);
            if (bufferRef != 0) {
                delete bufferRef;
            }
            bufferRef = buffer; // write pointer back.
            capacityRef = cap2; // write capacity back.
        }
        copy<T>(buf1, from1, len1, bufferRef, lengthRef);
        lengthRef = length3; // write length back.
    }

    template <typename T>
    static T *newArray(const T *buf1, const int from1, const int len1, const T *buf2, const int from2, const int len2, const int capacity) {
        T *dest = new T[capacity];
        Util::copy<T>(buf1, from1, len1, dest);
        Util::copy<T>(buf2, from2, len2, dest, len1);
        return dest;
    }

    static char *newStr(const char *buf1, const int from1, const int len1, const char *buf2, const int from2, const int len2) {
        return newStr(buf1, from1, len1, buf2, from2, len2, len1 + len2 + 1);
    }
    static char *newStr(const char *buf1, const int from1, const int len1, const char *buf2, const int from2, const int len2, const int capacity) {
        int len3 = len1 + len2;
        char *ret = newArray(buf1, from1, len1, buf2, from2, len2, capacity);
        ret[len3] = END_OF_STR;
        return ret;
    }

    template <typename T>
    static void copy(const T *source, int from1, int len1, T *dest) {
        copy<T>(source, from1, len1, dest, 0);
    }

    template <typename T>
    static void copy(const T *source, int from1, int len1, T *dest, int from2) {

        for (int i = 0; i < len1; i++) {
            dest[from2 + i] = source[from1 + i];
        }
    }
    static int strLength(const char *str) {
        if (str == 0) {
            return 0;
        }
        for (int i = 0;; i++) {
            if (str[i] == END_OF_STR) {
                if (i > 10000) {
                    i = 0;
                }
                return i;
            }
        }
    }
    template <typename T>
    static T &ref(T &ref) {
        return ref;
    }

    static int makeCapacity(int cap, int deltaCap, int cap2) {

        if (cap < cap2) {
            cap = cap - (cap % deltaCap);
        }

        while (cap <= cap2) {
            cap += deltaCap;
        }
        return cap;
    }
};

} // namespace a8::util
