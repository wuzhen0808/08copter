#pragma once
#include "a8/util/net.h"

namespace a8::util::rf24 {
using namespace a8::util;

class Rf24Hosts {
    // static
    static int hostHashCode(String str) {
        return str % 16;
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

    int resolveNodeId(String host, int &nodeId, Result &res) {
        int nId = this->hostNodes->get(host, -1);
        if (nId == -1) {
            res << "cannot resolve node id with host:" << host;
            return -1;
        }
        nodeId = nId;
        return 1;
    }
    friend String &operator<<(String &str, Rf24Hosts *hosts) {
        return *hosts->hostNodes >> str;
    }
};

} // namespace a8::util::rf24