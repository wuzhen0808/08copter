#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
namespace a8::util {
using namespace a8::util;
class BitUtil {
public:
    static int writeBinary(Writer *writer, String binStr) {
        if (binStr.len() % 8 != 0) {
            return -1;
        }
        int ret = 0;
        for (int i = 0; i < binStr.len() / 8; i++) {
            char chI = 0;
            for (int j = 0; j < 8; j++) {
                int idx = (i + 1) * 8 - j - 1;
                if (idx >= binStr.len()) {
                    return -1;
                }
                char chJ = binStr.charAt(idx);

                chI = chI | (((unsigned char)(chJ == '0' ? 0x0 : 0x1)) << j);
            }
            writer->write(chI);
            ret++;
        }
        return ret;
    }

    static char getBit(char ch, int idx) {
        int bit = ((unsigned char)ch >> (8 - idx - 1)) & 0x1U;
        return bit == 0 ? '0' : '1';
    }

    static int charToBinary(Buffer<char> buf, String &binStr) {
        int ret = 0;
        for (int i = 0; i < buf.len(); i++) {
            char ch = buf.get(i);
            for (int j = 0; j < 8; j++) {
                binStr << getBit(ch, j);
            }
            ret++;
        }
        return ret;
    }
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
