#pragma once
#include "a8/gs/View.h"
#include "a8/gs/defines.h"
#include "a8/hal.h"
#include "a8/util.h"
#include "a8/util/curses.h"

namespace a8::gs {
using namespace a8::util::curses;

using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::hal;

class LogView : public Output, public View {
    ScrollWindow *scrollWindow;
    Output *sysOutput;
    bool removeLn = true;
    BufferLineReader *lReader;
    System* sys;
    void afterBridgeCreate(Bridge<GsSkeleton> *eventData) {
    }

public:
    LogView(Cdk *cdk, EventCenter *eventCenter, LoggerFactory *logFac, System* sys) : View(cdk, eventCenter, logFac) {
        this->sys = sys;
        this->scrollWindow = 0;
        this->lReader = new BufferLineReader();
    }

    void draw() override {
        if (this->scrollWindow == 0) {
            this->scrollWindow = cdk->newScrollWindow(CENTER, CENTER, 30, -2, "Log View", 200, true, true);
            this->sysOutput = sys->out;
            sys->out = this;
        }
        this->scrollWindow->draw();
    }
    ~LogView() {
        sys->out = sysOutput;
        delete scrollWindow;
    }

    void print(const String msg) override {
        lReader->append(msg);
        while (true) {
            String line;
            int ret = lReader->readLine(line);
            if (ret <= 0) {
                return;
            }
            append(line);
        }
    }

    void append(String line) {
        this->scrollWindow->append(line);
    }
};
} // namespace a8::gs