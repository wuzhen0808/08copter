#pragma once
#include "a8/util/Strings.h"
#include "a8/util/curses/Dialog.h"
#include "a8/util/curses/Entry.h"
#include "a8/util/curses/Label.h"
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
    void refresh() {
        refreshCDKWindow(window);
        refreshCDKScreen(screen);
    }

    ScrollWindow *newScrollWindow(int x, int y, int h, int w, String title, int saveLines, bool box, bool shadow) {
        CDKSWINDOW *widget = newCDKSwindow(screen,
                                           CENTER, CENTER, 20, -2,
                                           title.text(), saveLines,
                                           BL(box), BL(shadow));

        return new ScrollWindow(widget);
    }
    Entry *newEntry(int x, int y, String prompt) {
        int width = COLS - prompt.len() - 4;
        CDKENTRY *widget = newCDKEntry(screen, CENTER, BOTTOM,
                                       0, prompt.text(),
                                       A_BOLD | COLOR_PAIR(8),
                                       COLOR_PAIR(24) | '_',
                                       vMIXED, width, 1, 512, FALSE, FALSE);
        return new Entry(widget);
    }

    Dialog *newDialog(int x, int y, Buffer<String> message, Buffer<String> buttons) {
        Strings msg(message);
        Strings bts(buttons);

        CDKDIALOG *widget = newCDKDialog(screen, CENTER, CENTER,
                                         msg.content(), msg.len(),
                                         bts.content(), bts.len(),
                                         COLOR_PAIR(2) | A_REVERSE,
                                         TRUE,
                                         TRUE, FALSE);
        return new Dialog(widget);
    }

    Label *newLabel(int x, int y, Buffer<String> message) {
        if (message.isEmpty()) {
            message.append("<empty>");
        }
        Strings msg(message);

        CDKLABEL *widget = newCDKLabel(screen, x, y,
                                       msg.content(), msg.len(),
                                       TRUE, FALSE);
        if (widget == 0) {
            return 0;
        }
        return new Label(widget);
    }

    ~Cdk() {
        destroyCDKScreen(this->screen);
        endCDK();
    }

    void initColor() {
        initCDKColor();
    }
};
} // namespace a8::util::curses