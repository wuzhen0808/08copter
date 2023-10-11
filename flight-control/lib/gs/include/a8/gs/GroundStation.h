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
    EventCenter * ec;
    Foreground *fg;
    Background *bg;

public:
    GroundStation(int argc, char **argv, Links *links) : Component("gs") {
        this->rates.append(Rate::RUN)->append(Rate::RUN);
        this->links = links;
        
    }

    ~GroundStation() {
    }

    void populate(StagingContext *context) override {
        Component::populate(context);
        this->ec = new EventCenter();
        this->bg = new Background(links, ec, context->loggerFactory);
        this->fg = new Foreground(bg, ec, context->loggerFactory);
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

    void run(TickingContext *ticking) override {

        if (ticking->getGroup() == 0) {
            int ret = bg->run(ticking);
            delete bg;
            bg = 0;
        } else if (ticking->getGroup() == 1) {
            Result rst;
            int ret = fg->run(rst);
            delete fg;
            fg = 0;
            log("after fg delete");
            ticking->getStaging()->scheduler->endSchedule();//
            log("after scheduler end.");
            exit(0);
        } else {
            log("unknow group of ticking.");
        }
    }
    

    void postStart(StagingContext *context) {
    }
};

} // namespace a8::gs