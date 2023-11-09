#include "a8/hal/rf24/Rf24Node.h"
#include <RF24Network.h>

namespace a8::hal::nrf24 {

int Rf24Node::readInt(int &iv) {
    RF24NetworkHeader header;
    CodecUtil::readInt16
    this->network->read(header,);
}

int Rf24Node::read(char *buf, int len) {
    CodecUtil::readString
}

} // namespace a8::hal::nrf24