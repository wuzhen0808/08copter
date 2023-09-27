#pragma once
namespace a8::util {

class Util {
public:
    template <typename T>
    static void copy(const T *sourceStr, int from1, T *toBuff, int from2, int len) {
        for (int i = 0; i < len; i++) {
            toBuff[from2 + i] = sourceStr[from1 + i];
        }
    }
    static int strLength(const char *str) {
        for (int i = 0;; i++) {
            if (str[i] == '\0') {
                return i;
            }
        }
    }
};

} // namespace a8::util
