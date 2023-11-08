#include "a8/hal/rf24/Rf24Socket.h"
#include "a8/hal/rf24/Rf24Sockets.h"

namespace a8::hal::nrf24 {

Rf24Socket::Rf24Socket() {
}
Rf24Socket::~Rf24Socket() {
}
void Rf24Socket::close() {
    this->status = Free;
}

} // namespace a8::hal::nrf24