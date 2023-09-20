#include "a8/hal/socket/native/NativeSocket.h"
#include "a8/hal/Hal.h"
#include <ws2tcpip.h>
#include <winsock.h>
#include <winsock2.h>

namespace a8::hal::socket::native {
using a8::hal::socket::Connection;
NativeConnection *NativeConnection::connect(String host, int port) {
    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return 0;
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host.getText(), &serv_addr.sin_addr) <= 0) {
        S->out->println("Invalid address/ Address not supported.");
        return 0;
    }

    this->clientFileDescriptor = ::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    if (clientFileDescriptor < 0) {
        S->out->println("Connection Failed.");
        return 0;
    }

    return this;
}

void NativeConnection::close() {
    ::closesocket(clientFileDescriptor);
}

int NativeConnection::send(const char *buf, int size) {
    return ::send(sock, buf, size, 0);
}

void NativeConnection::receive(){

}

} // namespace a8::hal::socket::native