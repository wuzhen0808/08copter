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

    // static member funcs
    static char **toCharArrayArray(Buffer<String> buf) {
        char **ret = new char *[buf.length()];
        for (int i = 0; i < buf.len(); i++) {
            String str = buf[i];
            Lang::appendStr(ret[i], str.text(), 0, str.len());
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
        return StringUtil::split<String>(str, separator, [](String &s) { return s; });
    }

    template <typename T>
    static Buffer<T> split(String string, const char separator, T (*convert)(String &)) {

        String str;
        Buffer<T> ret;
        for (int i = 0; i < string.length(); i++) {
            if (string.charAt(i) == separator) {
                ret.append(convert(str));
                str = "";
                continue;
            }
            str.append(string.charAt(i));
        }
        ret.append(convert(str));
        return ret;
    }
};
} // namespace a8::util