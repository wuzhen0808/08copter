#pragma once
#include "a8/util/curses.h"

using namespace a8::util::curses;

namespace a8::gs {

class LogView : public Output {
    ScrollWindow *scrollWindow;

public:
    LogView(Cdk *cdk) {
        this->scrollWindow = cdk->newScrollWindow(CENTER, CENTER, 30, -2, "Log View", 200, true, true);
    }
    ~LogView() {
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