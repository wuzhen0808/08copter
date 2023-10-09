#pragma once

#include "a8/util.h"
#include "a8/util/curses/Widget.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class Dialog : public Widget<CDKDIALOG> {

public:
    Dialog(CDKDIALOG *widget) : Widget(widget) {
    }
    ~Dialog() {
        destroyCDKDialog(widget);
    }

    void draw() override {
        drawCDKDialog(widget, widget->obj.box);
    }

    int activate(EExitType &exitType) {
        int ret = activateCDKDialog(widget, 0);
        exitType = this->widget->exitType;
        return ret;
    }
};
} // namespace a8::util::curses