#pragma once

#include "a8/util/rf24/Rf24Connector.h"
#include "a8/util/rf24/Rf24Listener.h"
#include "a8/util/rf24/Rf24NetData.h"
#include "a8/util/rf24/Rf24Node.h"
#include "a8/util/rf24/Rf24Player.h"
#include "a8/util/rf24/Rf24Watcher.h"
#include "a8/util/rf24/Rf24Worker.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include "a8/util/net.h"

namespace a8::util::rf24 {
using namespace a8::util;
using namespace a8::util::sched;

class Rf24Sock : FlyWeight {

    int id;         // sock id
    Rf24Node *node; // local node
    int port;       // local port
    //
    System *sys;
    Scheduler *sch;
    Rf24Player *player;

public:
    Rf24Sock(int id, System *sys, Scheduler *sch, LoggerFactory *logFac) : FlyWeight(logFac, String() << "Rf24Sock(" << id << ")") {
        this->id = id;
        this->sys = sys;
        this->sch = sch;
        this->node = 0;
        this->port = 0;
        this->player = new Rf24Watcher(id, node, port, sys, sch, logFac);
    }

    ~Rf24Sock() {
    }

    int getId() {
        return this->id;
    }

    int changeRole(Role role, Result &res) {
        Role role1 = this->player->getRole();
        if (role1 != Role::Watcher) {
            res << "cannot change role from " << role1 << " to " << role;
            return -1;
        }
        Rf24Player *player2 = 0;
        int ret = -1;
        switch (role) {
        case Role::Connector: {
            player2 = new Rf24Connector(this->id, this->node, this->port, this->sys, this->sch, this->loggerFactory);
            ret = 1;
            break;
        }
        case Role::Listener: {
            player2 = new Rf24Listener(this->id, this->node, this->port, this->sys, this->sch, this->loggerFactory);
            ret = 1;
            break;
        }
        case Role::Worker: {
            player2 = new Rf24Worker(this->id, this->node, this->port, this->sys, this->sch, this->loggerFactory);
            ret = 1;
            break;
        }
        case Role::Watcher: {
            res << "un supported operation.";
            ret = -1;
        }
        default:
            ret = -1;
        }

        if (player2 != 0) {
            Role role1 = this->player->getRole();
            delete this->player;
            this->player = player2;
            log(String() << "role changed from " << role1 << " to " << this->player->getRole());
        }

        return ret;
    }
    template <typename T>
    int asRole(Role role, T *&player2, Result &res) {
        if (this->player->isRole(role)) {
            player2 = static_cast<T *>(this->player);
            return 1;
        }
        return changeRole(role, player2, res);
    }

    template <typename T>
    int changeRole(Role role, T *&player2, Result &res) {
        int ret = this->changeRole(role, res);
        if (ret < 0) {
            return ret;
        }
        player2 = static_cast<T *>(this->player);
        return ret;
    }

    Rf24Player *getPlayer() {
        return this->player;
    }

    template <typename T>
    int getPlayer(Role role, T *&player, Result &res) {
        Role role1 = this->player->getRole();
        if (role1 != role) {
            res << "role(" << role1 << ") is not expected(" << role << ").";
            return -1;
        }
        player = static_cast<T *>(this->player);
        return 1;
    }

    bool isBond() {
        return this->port != 0;
    }

    int getPort() {
        return this->port;
    }

    void unBind() {
        this->node = 0;
        this->port = 0;
    }
    void close() {
        // this->player->close();//TODO
    }
};

} // namespace a8::util::rf24