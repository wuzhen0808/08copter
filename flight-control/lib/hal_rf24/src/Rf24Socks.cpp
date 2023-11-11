#include "a8/hal/rf24/Rf24Socks.h"

namespace a8::hal::rf24 {

void closeAndDelete(Rf24Sock *sock) {
    sock->close();
    delete sock;
}

Rf24Socks::~Rf24Socks() {

    table->clear<Rf24Socks *>(this, [](Rf24Socks *this_, int k, Rf24Sock *v) {
        closeAndDelete(v);
    });
    delete this->table;
}

int Rf24Socks::create() {
    int sId = this->nextId++;
    Rf24Sock *s = new Rf24Sock(sId);
    this->table->set(sId, s);
    return sId;
}

int Rf24Socks::close(int id) {

    int len = table->remove<Rf24Socks *>(this, id, [](Rf24Socks *this_, int k, Rf24Sock *v) {
        closeAndDelete(v);
    });

    if (len == 0) {
        return 0;
    }
    return len;
}
Rf24Sock *Rf24Socks::get(int id, Result &res) {
    Rf24Sock *sock = this->get(id);
    if (sock == 0) {
        res << "no such sock id:" << id;
    }
    return sock;
}
Rf24Sock *Rf24Socks::get(int id) {
    return table->get(id, 0);
}

int Rf24Socks::connect(int sId, String host2, int port2, Result &res) {

    Rf24Sock *s = this->get(sId, res);
    if (s == 0) {
        return -1;
    }

    if (s->getPort() == 0) {
        int port1 = ports->randomPort();
        int ok = s->bind(this->node, port1, res);
        if (ok < 0) {
            ports->release(port1);
            return -2;
        }
    }

    int nodeId2 = -1;
    int ret = hosts->resolveNodeId(host2, nodeId2, res);
    if (ret < 0) {
        return -1;
    }

    return s->connect(nodeId2, port2, res);
}

int Rf24Socks::bind(int sId, String host, int port, Rf24Hosts *hosts, Result &res) {
    Rf24Sock *s = this->get(sId, res);
    if (s == 0) {
        return -1; // no such sock
    }
    int nodeId = -1;
    int ret = hosts->resolveNodeId(host, nodeId, res);
    if (ret < 0) {
        return -2; // no such host.
    }

    if (nodeId != this->node->getId()) {
        res << "cannot bind to remote node:" << nodeId << ",only local node(" << this->node->getId() << ") can be bind to.";
        return -3; // host node is not local node.
    }

    if (s->bind(node, port, res) < 0) {
        return -4;
    }
    return 1;
}
int Rf24Socks::listen(int sId, Result &res) {
    Rf24Sock *s = this->get(sId, res);
    if (s == 0) {
        return -1; //
    }
    return s->listen(res);
}

int Rf24Socks::accept(int sId, int &sId2) {
    Rf24Sock *s = this->get(sId);
    if (s == 0) {
        return -1; //
    }
    // TODO
    return -1;
}

} // namespace a8::hal::rf24