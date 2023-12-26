#pragma once

namespace a8::util {
typedef bool int1;
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef float float32;
typedef double float64;

class Lang {
private:
    void emptyMethod() const {
    }

public:
    static const char END_OF_STR = '\0';
    template <typename T>
    static void empty(T) {
    }

    template <typename T>
    static int getBit(T value, int idx) {
        return (value >> (sizeof(T) * 8 - idx - 1)) & 0x1;
    }

    static int low4(char ch) {
        return ch & 0x0F;
    }

    static char hex(int iHex) {
        if (iHex < 10) {
            return iHex + '0';
        }
        return iHex - 10 + 'A';
    }

    static int high4(char ch) {
        return (ch >> 4) & 0x0F;
    }

    static char high4Hex(char ch) {
        return hex(high4(ch));
    }

    static char low4Hex(char ch) {
        return hex(low4(ch));
    }

    template <typename T>
    static T cast(void *ptr) {
        return static_cast<T>(ptr);
    }

    static bool isLower(char ch) {
        return ch >= 'a' && ch <= 'z';
    }

    static bool isUpper(char ch) {
        return ch >= 'A' && ch <= 'Z';
    }

    static char toLower(char ch) {
        if (isUpper(ch)) {
            ch = ch - 'A' + 'a';
        }
        return ch;
    }

    static bool equalsIgnoreCase(char c1, char c2) {
        return toLower(c1) == toLower(c2);
    }

    template <typename T>
    static void replace(T **&ptr, int &len, T **ptr2, int len2, bool isArray) {
        free(ptr, len, isArray);
        ptr = ptr2;
        len = len2;
    }

    template <typename T>
    static void replace(T *&ptr, T *ptr2, bool isArray) {
        free(ptr, isArray);
        ptr = ptr2;
    }

    template <typename T>
    static void free(T **ptr, int len, bool isArray) {
        if (ptr == 0) {
            return;
        }
        for (int i = 0; i < len; i++) {
            free(ptr[i], isArray);
        }
        delete[] ptr;
    }

    template <typename T>
    static void free(T *&ptr) {
        free<T>(ptr, false);
    }

    template <typename T>
    static void free(T *&ptr, bool isArray) {
        if (ptr == 0) {
            return;
        }
        if (isArray) {
            delete[] ptr;
        } else {
            delete ptr;
        }
        ptr = 0;
    }

    static void illegalArgument(const char *msg) {
        static_cast<Lang *>(0)->emptyMethod();
    }

    template <typename T>
    static void fill(T *buf, int width, T fill) {
        for (int i = 0; i < width; i++) {
            buf[i] = fill;
        }
    }

    static void bug() {
        // std::cout << "bug" << std::endl;
    }

    template <typename T>
    static T &replace(T *newObj, T &ref) {
        T *tmp = &ref;
        ref = *newObj;
        delete tmp;
        return ref;
    }

    template <typename T>
    static T noneConvert(const T &value) {
        return value;
    }
    template <typename T>
    static void release(void *v) {
        delete static_cast<T *>(v);
    }

    template <typename T>
    static void staticCastDelete(void *p) {
        delete static_cast<T *>(p);
    }

    template <typename T>
    static void delete_(T *p) {
        delete p;
    }
};

} // namespace a8::util
