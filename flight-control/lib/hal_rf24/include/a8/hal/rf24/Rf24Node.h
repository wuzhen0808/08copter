#pragma once
#include "a8/hal/rf24/Rf24ChannelData.h"
#include "a8/util/net.h"

class RF24;
template <class T>
class ESBNetwork;

namespace a8::hal::rf24 {

class Rf24Node {
    int id;
    ESBNetwork<RF24> *network;

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
};

} // namespace a8::hal::rf24