#pragma once

#include "a8/util/rf24/Rf24NetData.h"
#include "a8/util/rf24/Rf24Player.h"

#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::util::rf24 {
using namespace a8::util;
using namespace a8::util::sched;
class Rf24Watcher : public Rf24Player {
    enum Status {
        Unknown,
        Connected
    };
    Status status = Unknown;

public:
    Rf24Watcher(int id, Rf24Node *&node, int &port, System *sys, Scheduler *sch, LoggerFactory *logFac) : Rf24Player(Role::Watcher, id, node, port, sys, sch, logFac) {
    }
    ~Rf24Watcher() {
    }
    /**
     * Three use case:
     * 1) before connect as a client.
     * 2) before listen as a listener.
     * 3) before connected as a server.
     */
    int doBind(Rf24Node *node, int port, Result &res) {
        if (this->node != 0) {
            res << "cannot rebind, sock is already bind to port(" << port << "@" << node->getId() << ")";
            return -1; // rebind not supported.
        }
        this->node = node;
        this->port = port;
        return 1;
    }
};

} // namespace a8::util::rf24