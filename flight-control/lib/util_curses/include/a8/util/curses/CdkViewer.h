#pragma once

#include "a8/util.h"
#include "a8/util/curses/defines.h"
#include "a8/util/curses/CdkScreen.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class CdkViewer {

    Buffer<char *> content;

public:
    CDKVIEWER *viewer;

    CdkViewer(CdkScreen *screen) {
        Rectangle rect = screen->getRectangle();
        Buffer<String> buttons;
        int buttonHighLight = 0;
        bool box = false;
        bool shadow = false;

        init(screen, rect, buttons, buttonHighLight, box, shadow);
    }
    CdkViewer(CdkScreen *screen, Rectangle rect,
              Buffer<String> buttons, int buttonHighLight,
              bool box,
              bool shadow) {
        init(screen, rect, buttons, buttonHighLight, box, shadow);
    }

    void init(CdkScreen *screen, Rectangle rect,
              Buffer<String> buttons, int buttonHighLight,
              bool box,
              bool shadow) {
        char **buttons2 = StringUtil::toCharArrayArray(buttons);
        viewer = newCDKViewer(screen->screen,
                              rect.x, rect.y, rect.h, rect.w,
                              buttons2,
                              buttons.len(),
                              buttonHighLight,
                              box, shadow);
    }

    void append(String line) {
        content.append(Lang::newStr(line.text(), 0, line.len()));
        setCDKViewerInfo(viewer, content.getAll(), content.len(), false);
    }
    
};
} // namespace a8::util::curses