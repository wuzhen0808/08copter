#pragma once
#include "a8/gs/View.h"
#include "a8/hal.h"
#include "a8/util/Output.h"
#include "a8/util/curses.h"

using namespace a8::util::curses;

using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::hal;
namespace a8::gs {

class LogView : public Output, public View {
    ScrollWindow *scrollWindow;
    Output *sysOutput;

public:
    LogView(Cdk *cdk, LoggerFactory *logFac) : View(cdk, logFac) {
        this->scrollWindow = 0;
    }
    void draw() override {
        if (this->scrollWindow == 0) {
            this->scrollWindow = cdk->newScrollWindow(CENTER, CENTER, 30, -2, "Log View", 200, true, true);
            this->sysOutput = S->out;
            S->out = this;
        }
        this->scrollWindow->draw();
    }
    ~LogView() {
        S->out = sysOutput;
        delete scrollWindow;
    }

    void print(const String &msg) override {
        append(msg);
    }

    void append(String line) {
        this->scrollWindow->append(line);
    }
};
} // namespace a8::gs