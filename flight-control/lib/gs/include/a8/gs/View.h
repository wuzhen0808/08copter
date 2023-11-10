#pragma once
#include "a8/gs/Background.h"
#include "a8/util.h"
#include "a8/util/curses.h"
#include "a8/util/net.h"

namespace a8::gs {
using namespace a8::util;
using namespace a8::util::curses;

class View : public FlyWeight {
protected:
    Cdk *cdk;
    EventCenter *eventCenter;
public:
    View(Cdk *cdk, EventCenter *ec, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->cdk = cdk;
        this->eventCenter = ec;
    }

    virtual ~View() {
    }
    
    virtual void draw() = 0;

};
} // namespace a8::gs