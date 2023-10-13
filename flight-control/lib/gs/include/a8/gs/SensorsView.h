#pragma once
#include "a8/gs/View.h"
#include "a8/hal.h"
#include "a8/link.h"
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

public:
    SensorsView(Cdk *cdk, EventCenter *ec, LoggerFactory *logFac) : View(cdk, ec, logFac) {
        this->label = 0;

        ec->add<SensorsData *, SensorsView *>(EventTypes::ON_SENSORS_DATA, this, [](SensorsData *sd, SensorsView *this_) {
            this_->onEvent(sd);
        });
    }
    void onEvent(SensorsData *sd) {
        // must consume event data will be freed after method return.

        bool need = this->setIfNeeded<double>(0, "Al", sd->altitude);

        need = need || this->setIfNeeded<Vector3f>(1, "Av", sd->angVel);
        if (need) {
            this->draw();
        }
    }

    void draw() override {
        Lang::free(this->label);
        if (this->label == 0) {
            this->label = cdk->newLabel(CENTER, RIGHT, lines);
        } else {
            this->label->setMessages(lines);
        }
        this->label->draw();
    }

    ~SensorsView() {
        Lang::free(label);
    }

    template <typename T>
    bool setIfNeeded(int idx, String name, T value) {
        String line = String() << "<L>" << name << ":" << value;
        return lines.setIfNeeded(idx, "", line);
    }
};
} // namespace a8::gs