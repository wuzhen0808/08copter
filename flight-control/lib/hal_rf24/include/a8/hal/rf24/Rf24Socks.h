#pragma once
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24Ports.h"
#include "a8/hal/rf24/Rf24Sock.h"
#include "a8/util/net.h"

namespace a8::hal::rf24 {

class Rf24Socks : public FlyWeight {
    int nextId;
    HashTable<int, Rf24Sock *> *table; // socks by id;
    HashTable<int, Rf24Sock *> *binds; // socks by port;
    Rf24Node *node;
    Rf24Hosts *hosts;
    Rf24Ports *ports;

    int handleData(Rf24NodeData *data, long timeout, Result &res);

    int handleConnectRequest(Rf24ConnectRequest *req, long timeout, Result &res);

    int handleUserData(Rf24UserData *userData, long timeout, Result &res);

public:
    Rf24Socks(Rf24Node *node, Rf24Hosts *hosts, Rf24Ports *ports, LoggerFactory *logFac) : FlyWeight(logFac) {
        this->node = node;
        this->hosts = hosts;
        this->ports = ports;
        this->nextId = 1;
        this->table = new HashTable<int, Rf24Sock *>([](int k) { return k % 32; });
        this->binds = new HashTable<int, Rf24Sock *>([](int k) { return k % 16; });
    }
    ~Rf24Socks();
    int create(System *sys, Scheduler * sch);
    int close(int id);
    Rf24Sock *get(int id);
    Rf24Sock *get(int id, Result &res);
    Rf24Sock *findByPort(int port);
    int connect(int sId, String host2, int port2, Result &res);
    int doBind(Rf24Sock *s, int port, Result &res);
    int bind(int sId, String host, int port, Rf24Hosts *hosts, Result &res);
    int listen(int sId, Result &res);
    int accept(int sId, int &sid2);
    int send(int sId, const char *buf, int len, Result &res);
    int receive(int sId, char *buf, int len, Result &res);
    int update(Result &res);
};

} // namespace a8::hal::rf24