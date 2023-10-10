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

    static Buffer<String> split(String str, const char separator) {
        return StringUtil::split(&str, separator);
    }

    static Buffer<String> split(String *str, const char separator) {
        return StringUtil::split<String>(str, separator, [](String &s) { return s; });
    }

    template <typename T>
    static Buffer<T> split(String string, const char separator, T (*convert)(String &)) {
        return StringUtil::split(&string, separator, convert);
    }

    template <typename T>
    static Buffer<T> split(String *string, const char separator, T (*convert)(String &)) {

        String str;
        Buffer<T> ret;
        for (int i = 0; i < string->length(); i++) {
            if (string->charAt(i) == separator) {
                ret.append(convert(str));
                str = "";
                continue;
            }
            str.append(string->charAt(i));
        }
        ret.append(convert(str));
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
        str.append(buf.getAll(), buf.len());
        return str;
    }
};
} // namespace a8::util