#pragma once
#include "a8/util/net.h"

class RF24;
template <class>
class ESBNetwork;

namespace a8::hal::rf24 {

class Rf24NetworkWrapper {
    int id;
    ESBNetwork<RF24> *network;

public:
    Rf24NetworkWrapper(ESBNetwork<RF24> *network) {
        this->network = network;
    }

    ~Rf24NetworkWrapper() {        
    }
    bool available();
    int peekSize();
    int read(int &nodeId, char *buf, int len);
    void write(int nodeId, char *buf, int len);
};

} // namespace a8::hal::nrf24