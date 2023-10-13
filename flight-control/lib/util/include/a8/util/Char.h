#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
namespace a8::util {
using namespace a8::util;
class Char {
public:
    static int binaryToChars(String binStr, Buffer<char> &buf) {
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
            buf.append(chI);
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
};

} // namespace a8::util
