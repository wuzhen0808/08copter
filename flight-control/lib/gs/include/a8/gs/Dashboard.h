#pragma once
#include "a8/util.h"

#include <ncurses/panel.h>

using namespace a8::util;

namespace a8::gs {

class Dashboard {
public:
    Dashboard() {
    }

    void start() {
        initscr();
        refresh();
    }
    void print(String msg) {
        printw(msg.text());
    }

    void shutdown() {
        endwin();
    }
};

} // namespace a8::gs