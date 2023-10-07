#pragma once
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class CdkScreen {
    WINDOW *window_;
    CDKSCREEN *screen_;

public:
    CdkScreen() {
        window_ = initscr();
        screen_ = initCDKScreen(window_);
    }

    ~CdkScreen() {
        destroyCDKScreen(this->screen_);
        endCDK();
    }
    void initColor() {
        initCDKColor();
    }
    void end() {
        endCDK();
    }

    CDKSCREEN *screen() {
        return screen_;
    }
    WINDOW *window() {
        return window_;
    }

    Rectangle rect() {
        int x = getbegx(window_);
        int y = getbegy(window_);
        int x2 = getmaxx(window_);
        int y2 = getmaxy(window_);
        return Rectangle(x, y, (x2 - x), (y2 - y));
    }
};
} // namespace a8::util::curses