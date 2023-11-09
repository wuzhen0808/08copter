#pragma once
#include "a8/util/net.h"

namespace a8::hal::nrf24 {
using namespace a8::util;

class Rf24Ports {
    int nextRandomPort;
    HashTable<int, bool> *ports;

public:
    Rf24Ports() {
        this->ports = new HashTable<int, bool>([](int k) { return k % 256; });
        this->nextRandomPort = 49151;
    }
    ~Rf24Ports() {
        delete this->ports;
    }

    int randomPort() {

        int ret = -1;
        for (int i = 49151; i < 65536; i++) {
            if (this->ports->contains(this->nextRandomPort)) {
                this->nextRandomPort++;
                if (this->nextRandomPort == 65536) {
                    this->nextRandomPort = 49151;
                }
                continue;
            }
            this->ports->set(this->nextRandomPort, true);
            ret = this->nextRandomPort++;
            break;
        }
        return ret;
    }

    void release(int port) {
        this->ports->remove(port);
    }
};

} // namespace a8::hal::nrf24