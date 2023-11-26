#include "a8/ts/Joysticks.h"

namespace a8::ts::example {
class JoysticksExample : FlyWeight {
public:
    System *sys;
    Joysticks *jss;

    JoysticksExample(System *sys, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->sys = sys;
    }

    int start(Result &res) {
        jss = new Joysticks(15, 16, 6, 17, 18, 5, sys, loggerFactory);
        jss->setup();

        return true;
    }
};
} // namespace a8::ts::example