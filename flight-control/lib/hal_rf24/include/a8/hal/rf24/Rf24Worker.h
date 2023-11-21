#pragma once

#include "a8/hal/rf24/Rf24NetData.h"
#include "a8/hal/rf24/Rf24Player.h"

#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
class Rf24Worker : public Rf24Player {
    enum Status {
        Unknown,
        Connected
    };
    Status status = Unknown;

public:
    Rf24Worker(int id, Rf24Node *&node, int &port, System *sys, Scheduler *sch, LoggerFactory *logFac) : Rf24Player(Role::Worker, id, node, port, sys, sch, logFac) {
    }
    ~Rf24Worker() {
    }

    int connectIn(int node2, int port2, Result &res) {
        this->node2 = node2;
        this->port2 = port2;
        this->status = Status::Connected;
        return 1;
    }
    bool isConnected() override{
        return this->status == Status::Connected;
    }
};

} // namespace a8::hal::rf24