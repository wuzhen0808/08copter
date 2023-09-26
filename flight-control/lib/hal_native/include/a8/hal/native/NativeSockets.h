#include "a8/util/Result.h"
#include "a8/util/Sockets.h"

#if defined(_WIN32)
// clang-format off
#include <winsock2.h>
#include <winsock.h>
// clang-format on
#include <winioctl.h>
#include <ws2tcpip.h>
#define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
#define GET_SOCKET_ERRNO() (WSAGetLastError())
#else
#error "not supported."
#endif
using namespace a8::util;
// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket

namespace a8::hal::native {

class NativeSockets : virtual public Sockets {

public:
    NativeSockets() {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        this->startUpError = WSAStartup(wVersionRequested, &wsaData);
        // S->out->println(String::format("Actual socket version:%i", wsaData.wVersion));
    }
    ~NativeSockets() {
        WSACleanup();
    }

    virtual void close(SOCK &sock) override {
        ::closesocket(sock);
    }
    virtual Result setBlocking(SOCK sock, bool blocking) {
        u_long mode = blocking ? 0 : 1;
        int ret = ioctlsocket(sock, FIONBIO, &mode);
        if (ret == 0) {
            return true;
        }

        return ret;
    }

    virtual Result connect(SOCK sock, String &host, int port) override {
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

    virtual Result bind(SOCK sock, String &address, int port) override {
        sockaddr_in name;
        int nameLen = sizeof(name);
        memset(&name, 0, nameLen);

        name.sin_family = AF_INET;
        name.sin_port = htons(port);
        name.sin_addr.s_addr = inet_addr(address.getText());
        int error = ::bind(sock, (struct sockaddr *)&name, nameLen);
        if (error != 0) {
            // S->out->println(String::format("Failed to bind sock(%i) on address(%s:%i).", sock, address.getText(), port));
            return error;
        }
        return true;
    }

    virtual Result listen(SOCK sock) override {
        int error = ::listen(sock, 5); // what's backlog?
        if (error != 0) {
            // S->out->println(String::format("Failed listen on sock:%i, error:%i", sock, error));
            return error;
        }
        // S->out->println(String::format(">>Success listen on sock:%i", sock));
        return true;
    }

    virtual SOCK accept(SOCK sock) override {
        // S->out->println(String::format(">>accept... on sock:%i", sock));

        int ret = ::accept(sock, 0, 0);
        if (ret == 0) {
            //  S->out->println("Failed accept next client.");
            return 0;
        }
        // S->out->println("New client connected in.");
        return ret;
    }

    bool send(SOCK sock, String &msg) override {
        // S->out->println(String::format("sending... message:%s", str->getText()));
        int sRst = ::send(sock, msg.getText(), msg.getLength(), 0);
        if (sRst == SOCKET_ERROR) {
            // S->out->println(String::format("failed to send, rst:%i", sRst));
            return false;
        } else {
            // S->out->println(String::format("success of send, rst:%i", sRst));
            return true;
        }
    }

    int receive(SOCK sock, char *buf, int bufLen) override {
        return ::recv(sock, buf, bufLen, 0);
    }

    int getLastError() override {
        return WSAGetLastError();
    }

    int startUpError;

    bool isReady() {
        return this->startUpError == 0;
    }
    SOCK socket() override {
        if (!this->isReady()) {
            return 0;
        }

        int ret = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (!IS_VALID_SOCKET(ret)) {
            int socketError = GET_SOCKET_ERRNO();
            // S->out->println(String::format("Failed to create socket:%i, error:%i", ret, socketError));
            return 0;
        }
        return ret;
    }

    virtual int select(Buffer<SOCK> *readSockets, Buffer<SOCK> *writeSockets, Buffer<SOCK> *exceptionSockets) override {
        fd_set setR;
        fd_set setW;
        fd_set setE;
        TIMEVAL timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int ret = ::select(0, &setR, &setW, &setE, &timeout);
        if (ret = 0) {
            return 0;
        } else if (ret == SOCKET_ERROR) {
            return -1;
        } else {
            return ret;
        }
    }
};
} // namespace a8::hal::native
