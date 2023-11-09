#pragma once
#include "a8/hal/rf24/Rf24NetworkWrapper.h"
#include "a8/util/net.h"

namespace a8::hal::nrf24 {

class Rf24Channel {
    int port1;
    int port2;
    Reader *reader;
    Queue<char> *queue;

public:
    Rf24Channel(int port1, int port2, Reader *reader, Queue<char> *queue) {
        this->port1 = port1;
        this->port2 = port2;
        this->reader = reader;
        this->queue = queue;
    }

    Queue<char> *getQueue() {
        return queue;
    }
};

} // namespace a8::hal::nrf24