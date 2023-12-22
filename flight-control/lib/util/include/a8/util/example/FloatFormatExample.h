#pragma once
#include "a8/util.h"
namespace a8::util::example {

class FloatFormatExample {

public:
    void assert(int &ret, const char *s, String s2, Result &res) {
        assert(ret, String(s), s2, res);
    }
    void assert(int &ret, String s1, String s2, Result &res) {
        if (s1 != s2) {
            ret = -1;
            res << s1 << "!=" << s2 << "\n";
        }
    }
    int run(Logger *logger, Result &res) {
        Format::AutoOffsetFloat format(6, 3);
        String s1;
        s1.setFloatFormat(&format);
        s1 << 1.23f;
        int ret = 1;
        Result res;
        assert(ret, "1.230", s1, res);
        if (ret < 0) {
            logger->error(res.errorMessage);
        }
    }
};
} // namespace a8::util::example
