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
    static void free(void *ptr) {
        free<T>(Lang::cast<T *>(ptr));
    }

    template <typename T>
    static void free(T *&ptr) {
        free(ptr, false);
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

    static char *newStr(const char *buf1, const int from1, const int len1) {
        char *ret = 0;
        int len = 0;
        int cap = 0;
        appendStr(ret, len, cap, 1, buf1, from1, len1);
        return ret;
    }

    static void appendStr(char *&bufferRef, const char *buf1) {
        appendStr(bufferRef, buf1, 0, strLength(buf1));
    }

    static void appendStr(char *&bufferRef, const char *buf1, const int from1, const int len1) {
        int len = 0;
        int cap = 0;
        appendStr(bufferRef, len, cap, 1, buf1, from1, len1);
    }

    /**
     * Append a string to another.
     * Create new space from heap if necessary.
     * The old space will be released if new space is created.
     * Caller should be the owner of the space, the old one or the new one.
     *
     */

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
    /**
     * This method read the elements from the source buffer and append them to the end of the target buffer.
     * If necessary, the destination buffer's capacity will be expanded by new array with a new capacity.
     * The old buffer will be deleted from heap.
     *
     * Note that there are 3 references here for writing the process result back.
     *
     *  1. bufferRef - the pointer ref, it's may 0 or a valid buffer which created in heap by new keyword.
     *  2. lengthRef - the length ref, indicate the length of meaningful content of the buffer.
     *  3. capacityRef - the capacity ref, indicate th total size of the buffer when it is created.
     *  4. deltaCap - if the capacity need to be expanded, the new capacity should increase by this argument.
     *  5. tailCap - this arg specify more reserved space for caller to use, for instance the string tail to fill.
     *  6,7,8. buf1,from1,len1 - the source buffer informations.
     *
     * The result are:
     *  1. the pointer ref will be untouched if the capacity is enough to fill the new content.
     *     It will be updated if a new array buffer created.
     *  2. the length ref, will be updated to the total length after filling.
     *  3. the capacity ref will be updated if new array created.
     *
     * For example, to append "abc" into a buffer which is initialized with a 0, null pointer.
     *
     * @code
     *  //destination
        char * buf2 = 0;
        int len2 = 0;
        int capacity2 = 0;
        //source
        const char buf1[] = "abc";
        int len1 = 3;
        //filling and write back the result by references.
        Lang::append(buf, len, capacity, 8, 0, buf1, 0, len1);
        //check results.
        assert buf2 != 0;
        assert len2 = len1;
        assert capacity2 = 8;
        //free the array space.
        if(buf2 != 0){
            delete buf2;
        }
     *
     *
     */
    template <typename T>
    static void append(T *&bufferRef, int &lengthRef, int &capacityRef, int deltaCap, int tailCap,
                       const T *buf1, const int from1, const int len1) {

        int length3 = lengthRef + len1;

        int cap2 = capacityRef;
        bool new_ = false;
        while (cap2 < length3 + tailCap) { // find a new capacity.
            cap2 += deltaCap;
            new_ = true;
        }
        if (new_) {
            T *buffer = new T[cap2];
            copy<T>(bufferRef, 0, lengthRef, buffer);
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
    static void copy(const T *source, int from1, int len1, T *dest) {
        copy<T>(source, from1, len1, dest, 0);
    }

    template <typename T>
    static void copy(const T *source, int from1, int len1, T *dest, int from2) {

        for (int i = 0; i < len1; i++) {
            dest[from2 + i] = source[from1 + i];
        }
    }

    template <typename T>
    static void shift(T *buf, int len, int shift) {
        if (shift == 0) {
            return;
        }
        for (int i = 0; i < len; i++) {
            if (shift < 0) { // shift left.
                buf[i + shift] = buf[i];
            } else { // shift right
                buf[len - 1 - i + shift] = buf[len - 1 - i];
            }
        }
    }

    static int strLength(const char *str) {
        if (str == 0) {
            return 0;
        }
        for (int i = 0;; i++) {
            if (str[i] == END_OF_STR) {
                return i;
            }
        }
    }
    template <typename T>
    static T &replace(T *newObj, T &ref) {
        T *tmp = &ref;
        ref = *newObj;
        delete tmp;
        return ref;
    }
    /**
     * 1. cap - the current cap of some buffer.
     * 2. deltaCap - increase by delta.
     * 3. cap2 - the required minimize capacity to meet.
     *
     * Return:
     * 1. a number large than cap2.
     */
    static int makeCapacity(int cap, int deltaCap, int cap2) {

        if (cap < cap2) {
            cap = cap - (cap % deltaCap);
        }

        while (cap <= cap2) {
            cap += deltaCap;
        }
        return cap;
    }
    template <typename T>
    static void free(T **a2, int len) {
        for (int i = 0; i < len; i++) {
            delete[] a2[i];
        }
        if (a2 != 0) {
            delete[] a2;
        }
    }

    template <typename T>
    static T noneConvert(const T &value) {
        return value;
    }

    template <typename T>
    static int toIntBinaries(const T *value, int len1, char *str2, int len2) {
        int len11 = len1 * sizeof(T) * 8;
        int ret = -1;
        if (len2 < len11) {
            return ret; // no space enough to storage the result.
        }

        for (int i = 0; i < len1; i++) {
            ret = toIntBinary(*value[i], str2, i * sizeof(T) * 8, sizeof(T));
        }
        return len11;
    }

    template <typename T>
    static int parseIntBinaries(T *intVs, int len1, const char *str, int from, int len2) {

        if (len2 != sizeof(T) * 8 * len1) {
            return -1;
        }
        for (int i = 0; i < len1; i++) {

            intVs[i] = parseIntBinary(&intVs[i], str, from + sizeof(T) * 8 * i, sizeof(T) * 8);
        }
    }
};

} // namespace a8::util
