#pragma once

#include "a8/util.h"
#include "a8/util/curses/CdkScreen.h"
#include "a8/util/curses/defines.h"
#include <cdk.h>

namespace a8::util::curses {

class CdkViewer {

    Buffer<char *> content;
    CDKVIEWER *viewer;
    String title;

public:
    CdkViewer(CdkScreen *screen) {
        Buffer<String> buttons;
        buttons.append("</5><OK><!5>");
        buttons.append("</5><Cancel><!5>");
        bool box = TRUE;
        bool shadow = FALSE;
        Rectangle rect = screen->rect();
        init(screen, rect, buttons, box, shadow);
    }
    CdkViewer(CdkScreen *screen, Rectangle rect,
              Buffer<String> buttons,
              bool box,
              bool shadow) {
        init(screen, rect, buttons, box, shadow);
    }
    ~CdkViewer() {
        destroyCDKViewer(viewer);
    }

    void init(CdkScreen *screen, Rectangle rect,
              Buffer<String> buttons,
              bool box,
              bool shadow) {
        this->title = "<C></B/22>Log View<!22!B>";
        char **buttons2 = StringUtil::toCharArrayArray(buttons);
        viewer = newCDKViewer(screen->screen(),
                              CENTER, CENTER, 20, -2,
                              buttons2,
                              buttons.len(),
                              A_REVERSE,
                              box ? TRUE : FALSE, shadow ? TRUE : FALSE);
    }

    void activate() {
        int button = activateCDKViewer(this->viewer, 0);        
    }
    
    EExitType exitType(){
        return viewer->exitType;
    }

    void append(String line) {
        content.append(Lang::newStr(line.text(), 0, line.len()));

        setCDKViewer(viewer, title.text(), content.getAll(), content.len(), A_REVERSE, TRUE, TRUE, TRUE);
    }
};
} // namespace a8::util::curses