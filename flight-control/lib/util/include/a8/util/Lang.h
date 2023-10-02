#pragma once
#include <iostream>
namespace a8::util {

class Lang {
public:
    static const char END_OF_STR = '\0';
    /**
     * Format and append the formatted string to the buffer.
     *
     * Write back the results by reference.
     *
     * The buffer must be a space created by new in heap or zero in which case a new one will be created.
     *
     */
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

            capacity = Lang::makeCapacity(capacity, deltaCap, lenLeft + lenRight + 1);
            newBuf = new char[capacity];
            Lang::copy<char>(buf, 0, lenLeft, newBuf);
            delete[] buf;
            buf = newBuf;

            capRight = capacity - lenLeft;
            bufRight = buf + lenLeft;
            lenRight = snprintf(bufRight, capRight, formatStr, args...);

            if (lenRight + 1 > capRight) {
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

    static void bug() {
        std::cout << "bug" << std::endl;
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
};

} // namespace a8::util
