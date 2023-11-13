#pragma once
#include "a8/util/Result.h"

namespace a8::util {
/**
 */
class Writer {
protected:
public:
    Writer() {
    }

    ~Writer() {
    }
    int write(const char ch) {
        return write(&ch, 1);
    }

    int writeUntil(const char *buf, char til) {
        int ret = 0;
        for (int i = 0;; i++) {
            if (buf[i] == til) {
                break;
            }

            ret += write(buf, 1);
        }
        return ret;
    }
    int write(String str) {
        return write(str.text(), str.length());
    }

    int write(const char *buf, int bufLen) {
        Result res;
        return write(buf, bufLen, res);
    }

    virtual int write(const char *buf, int bufLen, Result &res) = 0;

    Writer &operator<<(const char ch) {
        write(ch);
        return *this;
    }

    Writer &operator<<(const String str) {
        write(str);
        return *this;
    }
};

} // namespace a8::util