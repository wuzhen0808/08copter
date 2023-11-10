#pragma once
#include "a8/util/net.h"

namespace a8::hal::rf24 {
using namespace a8::util;

class Rf24Hosts {
    // static
    static int hostHashCode(String str) {

        if (str.len() == 0) {
            return 0;
        }
        int len = 10;
        if (len > str.len()) {
            len = str.len();
        }
        int sum = 0;
        for (int i = 0; i < len; i++) {
            sum += (int)str.get(i);
        }
        return sum % 16;
    }
    // dynamic
    HashTable<String, int> *hostNodes; // host nodes
public:
    Rf24Hosts() {
        this->hostNodes = new HashTable<String, int>(hostHashCode);
    }

    void set(String host, int nodeId) {
        this->hostNodes->set(host, nodeId);
    }

    int resolveNodeId(String host, int &nodeId) {
        int nId = this->hostNodes->get(host, -1);
        if (nId >= 0) {
            nodeId = nId;
        }
        return 1;
    }
};

} // namespace a8::hal::nrf24