#pragma once
#include "a8/gs/View.h"
#include "a8/hal.h"
#include "a8/util/Output.h"
#include "a8/util/curses.h"

using namespace a8::util::curses;

using namespace a8::util;
using namespace a8::util::comp;
using namespace a8::hal;
namespace a8::gs {

class SensorsView : public View {
    Label *label;
    Buffer<String> lines;
    Background * bg;

public:
    SensorsView(Cdk *cdk, Background *bg, LoggerFactory *logFac) : View(cdk, logFac) {
        this->bg = bg;
        this->label = 0;
    }

    void draw() override {
        if (this->label == 0) {
            this->label = cdk->newLabel(CENTER, RIGHT, lines);
        }else{
            this->label->setMessages(lines);
        }

        this->label->draw();
    }

    ~SensorsView() {
        delete label;
    }

    void set(int idx, String name, String value) {
        lines.set(idx, "", "<L>" << name << "<R>" << value);
    }

};
} // namespace a8::gs