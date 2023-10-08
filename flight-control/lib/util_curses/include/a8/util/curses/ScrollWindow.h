#pragma once

#include "a8/util.h"
#include "a8/util/curses/Widget.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class ScrollWindow : Widget<CDKSWINDOW> {

public:
    ScrollWindow(CDKSWINDOW *widget): Widget(widget) {
    }
    
    void draw() override{
        drawCDKSwindow(widget, widget->obj.box);
    }

    ~ScrollWindow() {
        destroyCDKSwindow(widget);
    }

    void append(String line) {
        addCDKSwindow(widget, line.text(), BOTTOM);
    }
};
} // namespace a8::util::curses