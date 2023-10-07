#pragma once
#include "a8/util/curses.h"

using namespace a8::util::curses;

namespace a8::gs {

class LogView : public Output {
    CdkViewer *cdkViewer;

public:
    LogView(CdkScreen *cdkScreen) {
        this->cdkViewer = new CdkViewer(cdkScreen);
    }

    void print(const String &msg) override {
        append(msg);
    }

    void append(String line) {
        this->cdkViewer->append(line);
    }
};
} // namespace a8::gs