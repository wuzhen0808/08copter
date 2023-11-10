
#include "a8/hal/native.h"

#if defined(_WIN32)
#include "a8/hal/native/winsock2windows.h"
#include <winsock.h>
#include <winioctl.h>
#include <ws2tcpip.h>
#define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
#define GET_SOCKET_ERRNO() (WSAGetLastError())
#else
#error "not supported."
#endif // end of _WIND32

namespace a8::hal::native {
using namespace a8::util;
using namespace a8::util::net;
// https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket


static void toSet(Buffer<SOCK> &socks, fd_set &set) {
    for (int i = 0; i < socks.length(); i++) {
        set.fd_array[0] = socks.get(i);
        set.fd_count = i + 1;
    }
}
static void toBuffer(fd_set &set, Buffer<SOCK> &socks) {
    socks.clear();
    for (int i = 0; i < set.fd_count; i++) {
        socks.append(set.fd_array[i]);
    }
}

NativeSockets::NativeSockets() {
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    this->startUpError = WSAStartup(wVersionRequested, &wsaData);
    // S->out->println(String::format("Actual socket version:%i", wsaData.wVersion));
}
NativeSockets::~NativeSockets() {
    WSACleanup();
}

void NativeSockets::close(SOCK &sock) {
    ::closesocket(sock);
}
/*
 Result setBlocking(SOCK sock, bool blocking) {
    u_long mode = blocking ? 0 : 1;
    int ret = ioctlsocket(sock, FIONBIO, &mode);
    if (ret == 0) {
        return true;
    }

    return ret;
}
*/

int NativeSockets::connect(SOCK sock, const String host, int port, Result &res) {
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host.text());
    serv_addr.sin_port = htons(port);

    int cRst = ::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (cRst == SOCKET_ERROR) {
        res.errorMessage << "Connection failed to address:" << host << ":" << port;
        return -1;
    }

    return true;
}

int NativeSockets::bind(SOCK sock, const String address, int port, Result &res) {
    sockaddr_in name;
    int nameLen = sizeof(name);
    memset(&name, 0, nameLen);

    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = inet_addr(address.text());
    int error = ::bind(sock, (struct sockaddr *)&name, nameLen);
    if (error != 0) {
        res.errorMessage << "cannot bind to address:" << address << ":" << port;
        return -1;
    }
    return 1;
}

int NativeSockets::listen(SOCK sock, Result &rst) {
    int error = ::listen(sock, 5); // what's backlog?
    if (error != 0) {
        rst.errorMessage << "cannot listen sock:" << sock << ",error:" << error;
        return error;
    }
    return 0;
}

int NativeSockets::accept(SOCK sock, SOCK &sockIn) {
    int ret = ::accept(sock, 0, 0);
    if (ret == 0) {
        return -1;
    }
    sockIn = ret;
    return ret;
}

bool NativeSockets::send(SOCK sock, const char *buf, int len) {
    // S->out->println(String::format("sending... message:%s", str->text()));
    int sRst = ::send(sock, buf, len, 0);
    if (sRst == SOCKET_ERROR) {
        // S->out->println(String::format("failed to send, rst:%i", sRst));
        return false;
    } else {
        // S->out->println(String::format("success of send, rst:%i", sRst));
        return true;
    }
}

int NativeSockets::receive(SOCK sock, char *buf, int bufLen) {
    return ::recv(sock, buf, bufLen, 0);
}

int NativeSockets::getLastError() {
    return WSAGetLastError();
}

int startUpError;

bool NativeSockets::isReady() {
    return this->startUpError == 0;
}
int NativeSockets::socket(SOCK &sock) {
    if (!this->isReady()) {
        return -1;
    }

    int ret = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (IS_VALID_SOCKET(ret)) {
        sock = ret;
        ret = 1;
    }

    ret = GET_SOCKET_ERRNO();
    return ret;
}

int NativeSockets::select(SOCK &sock) {
    Buffer<SOCK> buffer1;
    Buffer<SOCK> buffer2;
    Buffer<SOCK> buffer3;
    buffer1.append(sock);
    int ret = select(buffer1, buffer2, buffer3);
    if (ret == 0) {
        return 0;
    } else if (ret == -1) {
        return -1;
    } else {
        sock = buffer1.get(0);
        return 1;
    }
}
int NativeSockets::select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) {
    fd_set set1;
    fd_set set2;
    fd_set set3;
    toSet(buffer1, set1);
    toSet(buffer2, set2);
    toSet(buffer3, set3);

    TIMEVAL timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    int ret = ::select(0, &set1, &set2, &set3, &timeout);
    toBuffer(set1, buffer1);
    toBuffer(set2, buffer1);
    toBuffer(set3, buffer1);

    if (ret = 0) {
        return 0;
    } else if (ret == SOCKET_ERROR) {
        return -1;
    } else {

        return ret;
    }
}

} // namespace a8::hal::native
