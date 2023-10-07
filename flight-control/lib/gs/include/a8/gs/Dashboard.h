#pragma once
#include "a8/gs/LogView.h"
#include "a8/gs/NcursesReader.h"

#include "a8/hal.h"
#include "a8/util.h"
#include "a8/util/comp.h"

#include <cdk.h>
#include <ncurses/panel.h>

using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::hal;

namespace a8::gs {

class Dashboard : public FlyWeight {
    LineReader *lr = 0;
    NcursesReader *nr = 0;
    CdkScreen *cdkScreen;
    LogView *logView = 0;
    Output *sysOutput = 0;

public:
    Dashboard(LoggerFactory *logFac) : FlyWeight(logFac) {
    }
    void open() {
    }
    void xopen() {
        this->cdkScreen = new CdkScreen();
        logView = new LogView(this->cdkScreen);
        sysOutput = S->out;
        // take over the system output.
        S->out = logView;
    }
    void close() {
    }
    void xclose() {
        // restore the default system output.
        S->out = sysOutput;
        sysOutput = 0;
        delete this->logView;
        this->logView = 0;
        endwin();
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