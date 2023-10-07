#pragma once
#include "a8/util/curses/ScrollWindow.h"
#include "a8/util/curses/defines.h"

#include <cdk.h>

namespace a8::util::curses {

class Cdk {
    WINDOW *window;
    CDKSCREEN *screen;
    static boolean BL(bool value) {
        return value ? TRUE : FALSE;
    }

public:
    Cdk() {
        window = initscr();
        screen = initCDKScreen(window);
    }

    ScrollWindow *newScrollWindow(int x, int y, int h, int w, String title, int saveLines, bool box, bool shadow) {
        CDKSWINDOW *widget = newCDKSwindow(screen,
                                           CENTER, CENTER, 20, -2,
                                           title.text(), saveLines,
                                           BL(box), BL(shadow));

        return new ScrollWindow(widget);
    }
    ~Cdk() {
        destroyCDKScreen(this->screen);
        endCDK();
    }

    void initColor() {
        initCDKColor();
    }

    void end() {
        endCDK();
    }
};
} // namespace a8::util::curses