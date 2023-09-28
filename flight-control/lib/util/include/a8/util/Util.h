#pragma once
namespace a8::util {

class Util {
public:
    template <typename T>
    static void copy(const T *source, T *dest, int len) {
        copy<T>(source, 0, dest, 0, len);
    }
    template <typename T>
    static void copy(const T *source, int from1, T *dest, int from2, int len) {
        for (int i = 0; i < len; i++) {
            dest[from2 + i] = source[from1 + i];
        }
    }
    static int strLength(const char *str) {
        for (int i = 0;; i++) {
            if (str[i] == '\0') {
                return i;
            }
        }
    }
    template <typename T>
    static T &ref(T &ref) {
        return ref;
    }
};

} // namespace a8::util
