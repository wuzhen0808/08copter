#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
namespace a8::util {

class StringUtil {
public:
    static String space(int len) {
        String str;
        return space(str, len);
    }

    static String &space(String &str, int len) {
        for (int i = 0; i < len; i++) {
            str << " ";
        }
        return str;
    }
    static String buildStr(Buffer<char> buf) {
        String str;
        for (int i = 0; i < buf.len(); i++) {
            str.append(buf[i]);
        }
        return str;
    }
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

    static String toHexString(const char *buf, int len) {
        Buffer<char> buf2(buf, len);
        return toHexString(buf2);
    }

    static String toHexString(const Buffer<char> &buf) {
        String buf2;
        for (int i = 0; i < buf.len(); i++) {
            char ch = buf.get(i, 0);
            buf2.append(Lang::high4Hex(ch));
            buf2.append(Lang::low4Hex(ch));
        }
        return buf2;
    }

    static Buffer<char> toHexBuffer(const Buffer<char> &buf) {
        Buffer<char> buf2;
        for (int i = 0; i < buf.len(); i++) {
            char ch = buf.get(i, 0);
            buf2.append(Lang::high4Hex(ch));
            buf2.append(Lang::low4Hex(ch));
        }
        return buf2;
    }

    friend String &operator<<(String &str, const Buffer<char> &buf) {
        str.append(buf.buffer(), buf.len());
        return str;
    }
    
    friend String &operator<<(String &str, const Buffer<String> &buf) {
        for (int i = 0; i < buf.len(); i++) {
            str << buf.get(i, "") << ",";
        }
        return str;
    }
};

} // namespace a8::util