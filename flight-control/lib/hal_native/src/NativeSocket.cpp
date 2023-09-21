#include "a8/hal/socket/native/NativeSocket.h"
#include "a8/hal/Hal.h"

#if defined(_WIN32)
// clang-format off
#include <ws2tcpip.h>
#include <winsock.h>
// clang-format on
#define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
#define GET_SOCKET_ERRNO() (WSAGetLastError())
#else
#define IS_VALID_SOCKET(s) ((s) >= 0)
#define GET_SOCKET_ERRNO() (errno)
#endif

namespace a8::hal::socket::native {
using a8::hal::socket::Socket;
//////////////////////////////////////////////////
// NativeSocketFactory
//
NativeSocketFactory::NativeSocketFactory() {
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    this->startUpError = WSAStartup(wVersionRequested, &wsaData);
    S->out->println(String::format("Actual socket version:%i", wsaData.wVersion));
}

NativeSocketFactory::~NativeSocketFactory() {
    WSACleanup();
}

bool NativeSocketFactory::isReady() {
    return this->startUpError == 0;
}

Socket *NativeSocketFactory::socket() {
    if (!this->isReady()) {
        return 0;
    }

    int sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!IS_VALID_SOCKET(sock)) {
        int socketError = GET_SOCKET_ERRNO();
        S->out->println(String::format("Failed to create socket:%i, error:%i", sock, socketError));
        return 0;
    }
    return new NativeSocket(sock);
}

//////////////////////////////////////////////////
// NativeSocket::
NativeSocket::NativeSocket(int sock) {
    this->sock = sock;
}

NativeSocket::~NativeSocket() {
    ::closesocket(sock);
}

bool NativeSocket::connect(String host, int port) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host.getText());
    serv_addr.sin_port = htons(port);

    int cRst = ::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (cRst == SOCKET_ERROR) {
        S->out->println(String::format("Connection failed to host:%s with port:%i", host.getText(), port));
        return false;
    }

    return true;
}

bool NativeSocket::setBlocking(bool blocking) {
    return false;
}

bool NativeSocket::send(String *str) {
    S->out->println(String::format("sending... message:%s", str->getText()));
    int sRst = ::send(sock, str->getText(), str->getLength(), 0);
    if (sRst == SOCKET_ERROR) {
        S->out->println(String::format("failed to send, rst:%i", sRst));
        return false;
    } else {
        S->out->println(String::format("success of send, rst:%i", sRst));
        return true;
    }
}

int NativeSocket::receive(char *buf, int bufLen) {
    return ::recv(sock, buf, bufLen, 0);
}

} // namespace a8::hal::socket::native