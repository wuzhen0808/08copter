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

class Foreground : public FlyWeight {
    LineReader *lr = 0;
    NcursesReader *nr = 0;
    LogView *logView = 0;
    Output *sysOutput = 0;
    Cdk *cdk;

public:
    Foreground(LoggerFactory *logFac) : FlyWeight(logFac) {
    }
    void open() {
        this->cdk = new Cdk();
        this->cdk->initColor();
        logView = new LogView(cdk);
        sysOutput = S->out;
        // take over the system output.
        S->out = logView;
        // logView->activate();
    }
    void close() {
        // restore the default system output.
        S->out = sysOutput;
        sysOutput = 0;
        delete this->logView;
        this->logView = 0;
        cdk->end();
    }

    void print(String msg) {
        printw(msg.text());
    }
};

} // namespace a8::gs