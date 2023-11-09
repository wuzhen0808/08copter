#include "a8/hal/rf24/Rf24NetworkWrapper.h"
#include <RF24Network.h>

namespace a8::hal::nrf24 {

bool Rf24NetworkWrapper::available() {
    return network->available();
}

int Rf24NetworkWrapper::peekSize() {
    RF24NetworkHeader header;
    return network->peek(header);
}

int Rf24NetworkWrapper::read(int &nodeId1, char *buf, int len) {
    RF24NetworkHeader header;
    int ret = this->network->read(header, buf, len);
    nodeId1 = header.from_node;
    return ret;
}

void Rf24NetworkWrapper::write(int nodeId2, char *buf, int len) {
    RF24NetworkHeader header;
    header.to_node = nodeId2;
    this->network->write(header, buf, len);
}

} // namespace a8::hal::nrf24