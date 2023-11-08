#include "a8/hal/rf24/Rf24Sockets.h"
#include "a8/hal/rf24/Rf24Socket.h"

namespace a8::hal::nrf24 {

Rf24Sockets::Rf24Sockets() {
    this->sockets = new Buffer<Rf24Socket *>();
    
}
Rf24Sockets::~Rf24Sockets() {
    for (int i = 0; i < sockets->len(); i++) {
        Lang::free<Rf24Socket>(sockets->get(i));
    }
    delete this->sockets;
}

int Rf24Sockets::socket(SOCK &sock) {

    if (this->sockets->len() < 16) {

        Rf24Socket *s = new Rf24Socket();
        sockets->append(s);
        sock = sockets->len() - 1;
        return 1;
    }
    for (int i = 0; i < sockets->len(); i++) {
        Rf24Socket *s = sockets->get(i);

        if (s->status == Free) {
            s->status = Running;
            sock = i;
            return i;
        }
    }

    return -1;//failed to create new socket.
}

void Rf24Sockets::close(SOCK &sock) {
    if (sock >= 0 && sock < this->sockets->len()) {
        Rf24Socket *s = sockets->get(sock);
        s->close();
    }
}

int Rf24Sockets::connect(SOCK sock, const String host, int port, Result &res) {
    Rf24Socket  * s = sockets->get(sock);
    s->connect();
    return true;
}

int Rf24Sockets::bind(SOCK sock, const String address, int port, Result &res) {
    return -1;
}

int Rf24Sockets::listen(SOCK sock, Result &rst) {
    return -1;
}

int Rf24Sockets::accept(SOCK sock, SOCK &sockIn) {
    return -1;
}

bool Rf24Sockets::send(SOCK sock, const char *buf, int len) {
    return -1;
}

int Rf24Sockets::receive(SOCK sock, char *buf, int bufLen) {
    return -1;
}

int Rf24Sockets::getLastError() {
    return -1;
}
bool Rf24Sockets::isReady() {
    return -1;
}

int Rf24Sockets::select(SOCK &sock) {
    return -1;
}
int Rf24Sockets::select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) {
    return -1;
}
} // namespace a8::hal::nrf24