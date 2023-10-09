#pragma once
#include "a8/gs/Background.h"
#include "a8/util.h"
#include "a8/util/curses.h"
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::curses;
namespace a8::gs {

class View : public FlyWeight {
protected:
    Cdk *cdk;
public:
    View(Cdk *cdk, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->cdk = cdk;
    }

    virtual ~View() {
    }
    
    virtual void draw() = 0;

};
} // namespace a8::gs