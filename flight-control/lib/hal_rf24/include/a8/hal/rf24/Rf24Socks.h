#pragma once
#include "a8/hal/rf24/Rf24Hosts.h"
#include "a8/hal/rf24/Rf24Ports.h"
#include "a8/hal/rf24/Rf24Sock.h"
#include "a8/util/net.h"

namespace a8::hal::rf24 {

class Rf24Socks {
    int nextId;
    HashTable<int, Rf24Sock *> *table; // socks;
    Rf24Node *node;
    Rf24Hosts *hosts;
    Rf24Ports *ports;

public:
    Rf24Socks(Rf24Node *node, Rf24Hosts *hosts, Rf24Ports *ports) {
        this->node = node;
        this->hosts = hosts;
        this->ports = ports;
        this->nextId = 1;
        this->table = new HashTable<int, Rf24Sock *>([](int k) { return k % 256; });
    }
    ~Rf24Socks();
    int create();
    int close(int id);
    Rf24Sock *get(int id);
    Rf24Sock *get(int id, Result &res);
    int connect(int sId, String host2, int port2, Result &res);
    int bind(int sId, String host, int port, Rf24Hosts *hosts, Result &res);
    int listen(int sId, Result &res);
    int accept(int sId, int &sid2);
};

} // namespace a8::hal::rf24