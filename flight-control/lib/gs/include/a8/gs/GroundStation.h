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
        this->rate = Rate::ForEver;
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
    }

    void run(TickingContext *ticking) override {
        // this method is running in a new thread.
        fg->open(); // open dashboard view.
        Scheduler *sch = ticking->getStaging()->scheduler;
        sch->schedule([](void *bg) {
            static_cast<Background *>(bg)->run();
        },
                      bg);
        // Note,this method is blocking until exit the dashboard?
        // so don't calling this method before postStart(), since Application will calling startSchedule until start() stage.
        fg->activate();
        fg->close();
        bg->stop();
        //sch->endSchedule();
    }

    void postStart(StagingContext *context) {
    }
};

} // namespace a8::gs