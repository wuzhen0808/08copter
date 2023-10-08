#pragma once
#include "a8/gs/Background.h"
#include "a8/gs/Foreground.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
namespace a8::gs {

class GroundStation : public Component {

    Links *links;
    Foreground *fg;
    Background *bg;

public:
    GroundStation(int argc, char **argv, Links *links) : Component("gs") {
        this->rates.append(Rate::RUN);
        this->links = links;
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
        this->fg = new Foreground(context->loggerFactory);
        this->bg = new Background(links, context->loggerFactory);
        Result rst;
        int ret = this->bg->open(rst);
        if (ret < 0) {
            context->stop(rst.errorMessage);
            return;
        }
        // this method is running in a new thread.
        fg->open(); // open dashboard view.
        // Scheduler *sch = context->scheduler;
        // sch->schedule(bg, [](void *bg) {
        //     static_cast<Background *>(bg)->run();
        // });

    }

    void run(TickingContext *ticking) override {
        bg->run();
    }

    void postStart(StagingContext *context) {
    }
};

} // namespace a8::gs