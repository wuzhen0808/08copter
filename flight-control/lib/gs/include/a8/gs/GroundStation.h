#pragma once
#include "a8/common.h"
#include "a8/gs/Background.h"
#include "a8/gs/Foreground.h"
#include "a8/link.h"
#include "a8/util/comp.h"
#include "a8/util/net.h"

namespace a8::gs {
using namespace a8::util;
using namespace a8::util::net;
using namespace a8::util::comp;
using namespace a8::common;

class GroundStation : public Component {

    Links *links;
    EventCenter *ec;
    Foreground *fg;
    Background *bg;
    bool enableFg = true;
    int argc;
    char **argv;

public:
    GroundStation(int argc, char **argv, Links *links) : Component("gs") {
        this->argc = argc;
        this->argv = argv;
        this->schedule<GroundStation>([](TickingContext *tc, GroundStation *this_) {
            this_->runBg(tc);
        });
        this->schedule<GroundStation>([](TickingContext *tc, GroundStation *this_) {
            this_->runFg(tc);
        });
        this->links = links;
        this->fg = 0;
    }

    ~GroundStation() {
    }

    void boot(StagingContext *context) override {
        CommonUtil::resolveProperties(argc, argv, context->properties, context->getSys());
        Component::boot(context);
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
        this->enableFg = context->properties->getBool(P_gs_enable_fg, true);
        this->ec = new EventCenter();
        this->bg = new Background(links, ec, context->loggerFactory);
        if (this->enableFg) {
            this->fg = new Foreground(bg, ec, context->loggerFactory);
        }
        Result rst;
        int ret = this->bg->open(rst);
        if (ret < 0) {
            context->stop(rst.errorMessage);
            return;
        }
        // this method is running in a new thread.

        // Scheduler *sch = context->scheduler;
        // sch->schedule(bg, [](void *bg) {
        //     static_cast<Background *>(bg)->run();
        // });
    }

    void runBg(TickingContext *ticking) {
        int ret = bg->run(ticking);
        delete bg;
        bg = 0;
    }

    void runFg(TickingContext *ticking) {
        if (!this->enableFg) {
            return;
        }

        Result rst;
        int ret = fg->run(rst);
        delete fg;
        fg = 0;
        log("after fg delete");
        ticking->getStaging()->scheduler->endSchedule(); //
        log("after scheduler end.");
        exit(0);
    }
};

} // namespace a8::gs