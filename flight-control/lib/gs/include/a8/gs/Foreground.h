#pragma once
#include "a8/gs/CommandView.h"
#include "a8/gs/LogView.h"
#include "a8/gs/SensorsView.h"
#include "a8/util/curses.h"
#include "a8/hal.h"
#include "a8/util.h"
#include "a8/util/comp.h"

namespace a8::gs {
using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::hal;


class Foreground : public FlyWeight {
    LineReader *lr = 0;
    LogView *logView = 0;
    CommandView *cmdView = 0;
    SensorsView *sensorsView = 0;
    Cdk *cdk;
    Background *bg;
    EventCenter * eventCenter;

public:
    Foreground(Background *bg, EventCenter * eventCenter, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->bg = bg;
        this->eventCenter = eventCenter;
        this->cdk = new Cdk();
        this->cdk->initColor();
        logView = new LogView(cdk, eventCenter, logFac);
        cmdView = new CommandView(cdk, eventCenter, loggerFactory);
        sensorsView = new SensorsView(cdk, eventCenter, logFac);
    }

    ~Foreground() {
        // restore the default system output.
        delete this->cmdView;
        delete this->logView;
        delete this->sensorsView;
        delete this->cdk;        
    }

    int run(Result &rst) {
        logView->draw();
        cmdView->draw();
        sensorsView->draw();
        int ret = cmdView->run(rst);

        return ret;
    }

    void print(String msg) {
        printw(msg.text());
    }
};

} // namespace a8::gs