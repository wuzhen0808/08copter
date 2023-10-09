#pragma once

#include "a8/util.h"
#include "a8/util/curses/Widget.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class Label : public Widget<CDKLABEL> {

public:
    Label(CDKLABEL *widget) : Widget(widget) {
    }
    ~Label() {
        destroyCDKLabel(widget);
    }

    void setMessages(Buffer<String> lines) {
        Strings strs = lines;
        setCDKLabelMessage(widget, strs.content(), strs.len());
    }

    void draw() override {
        if (widget == 0) {
            return;
        }
        drawCDKLabel(widget, widget->obj.box);
    }
};
} // namespace a8::util::curses