#pragma once
#include "a8/gs/View.h"
#include "a8/util/curses.h"

using namespace a8::util::curses;

namespace a8::gs {

class DialogView : public View {

    using buttonCb = void (*)(int); // button callback

    Dialog *dialog;
    Buffer<String> message;
    Buffer<String> buttons;

public:
    DialogView(Cdk *cdk, EventCenter *ec, LoggerFactory *logFac) : View(cdk, ec, logFac) {
        this->dialog = 0;
    }

    ~DialogView() {
        delete dialog;
    }

    void draw() override {
        this->getOrCreate()->draw();
    }
    Dialog *getOrCreate() {
        if (this->dialog == 0) {
            this->dialog = cdk->newDialog(CENTER, CENTER, message, buttons);
        }
        return this->dialog;
    }

    DialogView &operator<<(String msg) {
        message.append(msg);
        return *this;
    }

    void addButton(String button) {
        buttons.append(button);
    }

    int activate(EExitType &exitType) {
        return this->dialog->activate(exitType);
    }
};
} // namespace a8::gs