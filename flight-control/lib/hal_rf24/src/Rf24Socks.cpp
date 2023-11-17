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

int Rf24Socks::close(int id) {
    int len = table->remove<Rf24Socks *>(this, id, [](Rf24Socks *this_, int k, Rf24Sock *v) {
        this_->destroy(v);
    });
    return 1;
}

void Rf24Socks::destroy(Rf24Sock *s) {
    int port = s->getPort();

    if (port > 0) {
        s->unBind();
        this->ports->release(port);
    }

    s->close();

    delete s;
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
        int ok = this->doBind(s, port1, res);
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
    return this->doBind(s, port, res);
}

int Rf24Socks::listen(int sId, Result &res) {
    Rf24Sock *s = this->get(sId, res);
    if (s == 0) {
        return -1; //
    }
    return s->listen(res);
}

int Rf24Socks::accept(int sock, int &sockIn, Result &res) {
    Rf24Sock *s = this->get(sock, res);
    if (s == 0) {
        return -1; //
    }
    Rf24NetData *req = s->takeByType(Rf24NetData::TYPE_CONNECT_REQUEST);
    Rf24Sock *s2 = 0;
    int ret = this->doAccept(req, s2, res);
    delete req;
    if (ret < 0) {
        return ret;
    }
    sockIn = s2->getId();
    return ret;
}

int Rf24Socks::send(int sId, const char *buf, int len, Result &res) {
    Rf24Sock *s = this->get(sId, res);
    if (s == 0) {
        return -1; //
    }
    return s->send(buf, len, res);
}

int Rf24Socks::receive(int sId, char *buf, int len, Result &res) {
    Rf24Sock *s = this->get(sId, res);
    if (s == 0) {
        return -1; //
    }

    return s->receive(buf, len, res);
}

Rf24Sock *Rf24Socks::findByPort(int port) {
    return this->binds->get(port, 0);
}

} // namespace a8::hal::rf24