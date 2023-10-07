#pragma once
#include <cdk.h>
#include "a8/util/curses/defines.h"

namespace a8::util::curses {

class CdkScreen {
    WINDOW *window;

public:
    CDKSCREEN *screen;
    CdkScreen() {
        window = initscr();
        screen = initCDKScreen(window);
        initCDKColor();
    }
    
    Rectangle getRectangle() {
        int x = getbegx(window);
        int y = getbegy(window);
        int x2 = getmaxx(window);
        int y2 = getmaxy(window);

        return Rectangle(x, y, (x2 - x), (y2 - y));
    }

    ~CdkScreen() {
    }
};
} // namespace a8::util::curses