#pragma once

#include "a8/util/rf24/Rf24NetData.h"
#include "a8/util/rf24/Rf24Node.h"
#include "a8/util/rf24/Rf24Player.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::util::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
class Rf24Listener : public Rf24Player {
    enum Status {
        Unknown,
        Listening
    };
    Status status = Unknown;

public:
    Rf24Listener(int id, Rf24Node *&node, int &port, System *sys, Scheduler *sch, LoggerFactory *logFac) : Rf24Player(Role::Listener, id, node, port, sys, sch, logFac) {
    }
    ~Rf24Listener() {
    }
    /**
     * Get ready for incoming connect.
     */
    int listen(Result &res) {
        this->status = Status::Listening;
        return 1;
    }
};

} // namespace a8::util::rf24