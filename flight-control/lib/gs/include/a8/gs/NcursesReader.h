#pragma once
#include "a8/util/Reader.h"
#include <ncursesw/curses.h>

using namespace a8::util;

namespace a8::gs {
class NcursesReader : public Reader {
    int read(char *buf, int len) override {
        for (int i = 0; i < len; i++) {
            buf[i] = getch();
            if (buf[i] == '\n') {
                return i;
            }
        }
        
        return len;
    }
};
} // namespace a8::gs