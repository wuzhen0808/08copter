#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#define DELTA_STR_CAP (16)
namespace a8::util {

class StringUtil {
public:
    static String buildStr(Buffer<String> buffer, char separator) {
        String ret;
        int len = buffer.len();
        for (int i = 0; i < len; i++) {
            ret << separator << buffer[i];
        }
        return ret;
    }

    static Buffer<String> strings(int argc, char **argv) {
        Buffer<String> ret;
        for (int i = 0; i < argc; i++) {
            String str = String::string(argv[i]);
            ret.append(str);
        }
        return ret;
    }

    static Buffer<String> split(const String &str, const char separator) {
        return StringUtil::split<String>(str, separator, Lang::noneConvert<String>);
    }

    template <typename T>
    static Buffer<T> split(const String &string, const char separator, T (*convert)(const String &)) {

        String str;
        Buffer<T> ret;
        for (int i = 0; i < string.length(); i++) {
            if (string.charAt(i) == separator) {
                ret.template append<String>(str, convert);
                str = "";
                continue;
            }
            str.append(string.charAt(i));
        }
        ret.template append<String>(str, convert);
        return ret;
    }
    static String toString(Buffer<char> buf) {
        String str;
        for (int i = 0; i < buf.len(); i++) {
            char ch = buf[i];
            if (ch == '\0') {
                break;
            }
            str.append(ch);
        }
        return str;
    }

    friend String &operator<<(String &str, Buffer<char> buf) {
        str.append(buf.buffer(), buf.len());
        return str;
    }
};
} // namespace a8::util