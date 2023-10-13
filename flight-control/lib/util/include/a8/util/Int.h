#pragma once

#include "a8/util/String.h"
namespace a8::util {
using namespace a8::util;
class Int {
public:
    template <typename T>
    static int toBinaries(Buffer<T> values, String &bin) {
        int len = 0;
        for (int i = 0; i < values.len(); i++) {
            int ret = toBinary<T>(values[i], bin);
            if (ret < 0) {
                return ret;
            }
            len += ret;
        }
        return len;
    }

    template <typename T>
    static int parseBinaries(String binStr, Buffer<T> &buf, int expectResultLen) {
        if (binStr.len() != expectResultLen * sizeof(T) * 8) {
            return -1;
        }
        for (int i = 0; i < expectResultLen; i++) {
            T value;
            int ret = parseBinary(binStr, i * sizeof(T) * 8, value);
            if (ret < 0) {
                return ret;
            }
            buf.append(value);
        }
        return buf;
    }

    template <typename T>
    static int toBinary(T value, String &bin) {
        int len = sizeof(T) * 8;
        char buf[len];
        int ret = Lang::toIntBinary<T>(value, buf, len);
        if (ret < 0) {
            return ret;
        }
        bin << String(buf, len);
        return len;
    }

    template <typename T>
    static int parseBinary(const String &str, int from, T &intV) {
        const char *text = str.text();
        if (str.len() < from + sizeof(T) * 8) {
            return -1;
        }
        return Lang::parseIntBinary<T>(intV, str.text(), from, sizeof(T) * 8);
    }
};

} // namespace a8::util
