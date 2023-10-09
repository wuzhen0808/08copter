#pragma once

#include "a8/util.h"
#include "a8/util/curses/Widget.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class Entry : public Widget<CDKENTRY> {

public:
    Entry(CDKENTRY *widget) : Widget(widget) {
    }
    ~Entry() {
        destroyCDKEntry(widget);
    }

    void draw() override {
        drawCDKEntry(widget, widget->obj.box);
    }

    int activate(String &command) {
        char *str = activateCDKEntry(widget, 0);
        command = str;
        return command.len();
    }
    void clean(){
        cleanCDKEntry(widget);
    }

    EExitType exitType() {
        return widget->exitType;
    }
    
};
} // namespace a8::util::curses