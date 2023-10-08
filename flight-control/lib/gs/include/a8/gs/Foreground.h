#pragma once
#include "a8/gs/CommandView.h"
#include "a8/gs/LogView.h"

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
    LogView *logView = 0;
    CommandView *cmdView = 0;
    Output *sysOutput = 0;
    Cdk *cdk;
    Background *bg;

public:
    Foreground(Background *bg, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->bg = bg;

        this->cdk = new Cdk();
        this->cdk->initColor();
        logView = new LogView(cdk);
        cmdView = new CommandView(cdk, bg, loggerFactory);
        sysOutput = S->out;
        // take over the system output.
        S->out = logView;
    }

    ~Foreground() {
        // restore the default system output.
        S->out = sysOutput;
        sysOutput = 0;
        delete this->cmdView;
        delete this->logView;
        delete this->cdk;
    }

    int run(Result &rst) {
        
        int ret = cmdView->run(rst);
        

        return ret;
    }


    void print(String msg) {
        printw(msg.text());
    }
};

} // namespace a8::gs