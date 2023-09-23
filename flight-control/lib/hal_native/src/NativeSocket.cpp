#include "a8/hal/native/NativeSocket.h"
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

namespace a8::hal::native {

//////////////////////////////////////////////////
// NativeSocket::
NativeSocket::NativeSocket(int sock) {
    this->sock = sock;
}

NativeSocket::~NativeSocket() {
    ::closesocket(sock);
}

bool NativeSocket::setBlocking(bool blocking) {
    return false;
}

bool NativeSocket::send(String *str) {
    //S->out->println(String::format("sending... message:%s", str->getText()));
    int sRst = ::send(sock, str->getText(), str->getLength(), 0);
    if (sRst == SOCKET_ERROR) {
        S->out->println(String::format("failed to send, rst:%i", sRst));
        return false;
    } else {
        //S->out->println(String::format("success of send, rst:%i", sRst));
        return true;
    }
}

int NativeSocket::receive(char *buf, int bufLen) {
    return ::recv(sock, buf, bufLen, 0);
}

int NativeSocket::getLastError() {
    return WSAGetLastError();
}
//////////////////////////////////////////////////
// SocketClient
Result NativeSocketClient::connect(String host, int port) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host.getText());
    serv_addr.sin_port = htons(port);

    int cRst = ::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (cRst == SOCKET_ERROR) {
        return Result(cRst, String::format("Connection failed to host:%s with port:%i", host.getText(), port));
    }

    return true;
}
//////////////////////////////////////////////////
// SocketServer

Result NativeSocketServer::bind(String address, int port) {
    sockaddr_in name;
    int nameLen = sizeof(name);
    memset(&name, 0, nameLen);

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = inet_addr(address.getText());
    int error = ::bind(this->sock, (struct sockaddr *)&name, nameLen);
    if (error != 0) {
        S->out->println(String::format("Failed to bind sock(%i) on address(%s:%i).", sock, address.getText(), port));
        return error;
    }
    return true;
}

Result NativeSocketServer::listen() {
    int error = ::listen(sock, 5); // what's backlog?
    if (error != 0) {
        S->out->println(String::format("Failed listen on sock:%i, error:%i", this->sock, error));
        return error;
    }
    S->out->println(String::format(">>Success listen on sock:%i", this->sock));
    return true;
}

Socket *NativeSocketServer::accept() {
    S->out->println(String::format(">>accept... on sock:%i", this->sock));

    int sock2 = ::accept(this->sock, 0, 0);
    if (sock2 == 0) {
        S->out->println("Failed accept next client.");
        return 0;
    }
    S->out->println("New client connected in.");
    return new NativeSocket(sock2);
}

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

SocketServer *NativeSocketFactory::socketServer() {
    int sock = socket();
    return new NativeSocketServer(sock);
}
int NativeSocketFactory::socket() {
    if (!this->isReady()) {
        return 0;
    }

    int sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!IS_VALID_SOCKET(sock)) {
        int socketError = GET_SOCKET_ERRNO();
        S->out->println(String::format("Failed to create socket:%i, error:%i", sock, socketError));
        return 0;
    }
    return sock;
}
SocketClient *NativeSocketFactory::socketClient() {
    int sock = socket();
    return new NativeSocketClient(sock);
}

} // namespace a8::hal::native