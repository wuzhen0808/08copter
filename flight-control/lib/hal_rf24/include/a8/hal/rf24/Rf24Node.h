#pragma once
#include "a8/hal/rf24/Rf24Message.h"
#include "a8/util/net.h"

class RF24;
template class ESBNetwork<class T>;

namespace a8::hal::nrf24 {

class Rf24Node {
    int id;
    ESBNetwork<RF24> *network;
    int readInt(int &iv);
    int read(char* buf, int len);

public:
    Rf24Node(int id, ESBNetwork<RF24> *network) {
        this->id = id;
        this->network = network;
    }
    ~Rf24Node() {
    }
    
    int getId() {
        return id;
    }

    template <typename C>
    int read(C c, void (*consumer)(Rf24ChannelFrame *)) {
        int port2;
        int ret = readInt(port2);
        if (ret < 0) {
            return ret;
        }
        int size;
        ret = readInt(size);
        if (ret < 0) {
            return ret;
        }
        char *buf = new char[size];

        ret = read(buf, size);
        if (ret < 0) {   
            delete [] buf;         
            return ret;
        }

        Rf24ChannelFrame *msg = new Rf24ChannelFrame(port2, size, buf);
        consumer(msg);
        delete msg;
    }

    int write(int nodeId, Rf24ChannelFrame *msg) {

    }
};

} // namespace a8::hal::nrf24