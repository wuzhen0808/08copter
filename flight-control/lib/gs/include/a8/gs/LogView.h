#pragma once
#include "a8/util/curses.h"

using namespace a8::util::curses;

namespace a8::gs {

class LogView : public Output {
    CdkViewer *cdkViewer;

public:
    LogView(CdkScreen * screen) {        
        this->cdkViewer = new CdkViewer(screen);
    }
    ~LogView() {
        delete cdkViewer;
    }

    void activate(){
        cdkViewer->activate();
    }

    void print(const String &msg) override {
        append(msg);
    }

    void append(String line) {
        this->cdkViewer->append(line);
    }
};
} // namespace a8::gs