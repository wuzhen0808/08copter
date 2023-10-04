#pragma once
#include "a8/gs/NcursesReader.h"
#include "a8/util.h"
#include "a8/util/comp.h"

#include <ncurses/panel.h>

using namespace a8::util;
using namespace a8::util::comp;

namespace a8::gs {

class Dashboard : public Component {
    LineReader *lr;

public:
    Dashboard() : Component("dashboard") {
        this->rate = Rate::ForEver;
        lr = new LineReader(new NcursesReader());
    }

    void setup(StagingContext *context) {
        //initscr();
        //echo();
        //cbreak();
        //keypad(stdscr, TRUE);
        //print("a8 Ground Station");
        //refresh();
    }

    void xrun(TickingContext *context) {
        while (true) {

            String cmd;
            int len = lr->readLine(cmd);
            if (len <= 0) {
                log("error when read command from ncurses.");
                break;
            }
            if (cmd == "quit" || cmd=="exit") {
                log("quit from gs.");
                break;
            } else if (cmd == "endwin") {
                endwin();
            } else if (cmd == "ping fc") {
                
            } else {
            }
        }
    }
    void print(String msg) {
        //printw(msg.text());
    }

    void shutdown(StagingContext *context) {
        //endwin();
    }
};

} // namespace a8::gs