#pragma once
#include "a8/gs/NcursesReader.h"
#include "a8/util.h"
#include "a8/util/comp.h"

#include <ncurses/panel.h>

using namespace a8::util;
using namespace a8::util::comp;

namespace a8::gs {

class Dashboard : public FlyWeight {
    LineReader *lr = 0;
    NcursesReader *nr = 0;

public:
    Dashboard(LoggerFactory *logFac) : FlyWeight(logFac) {
    }

    void open() {
        initscr();
        echo();
        cbreak();
        keypad(stdscr, TRUE);
        print("a8 Ground Station");
        refresh();
        nr = new NcursesReader();
        lr = new LineReader(nr);
    }

    void close() {

        endwin();

        if (nr != 0) {
            delete nr;
        }
        if (lr != 0) {
            delete lr;
        }
    }
    
    template <typename T>
    int receive(int len, void (*handle)(T, String), T *context, Result &rst) {
        int len_ = 0;
        int ret = -1;
        while (len == -1 || len_ < len) {
            String cmd;
            ret = lr->readLine(cmd, rst);
            if (ret < 0) {
                break;
            }
            handle(context, cmd);
        }
        return ret;
    }

    void print(String msg) {
        printw(msg.text());
    }
};

} // namespace a8::gs