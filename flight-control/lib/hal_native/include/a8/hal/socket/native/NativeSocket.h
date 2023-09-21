#include "a8/hal/socket.h"
namespace a8::hal::socket::native {

using a8::hal::socket::Socket;
using a8::hal::socket::SocketFactory;

class NativeSocket;

class NativeSocketFactory : public SocketFactory {
private:
    int startUpError;

public:
    NativeSocketFactory();
    ~NativeSocketFactory();
    bool isReady();
    virtual Socket *socket();
};

class NativeSocket : public Socket {
private:
    int sock;

public:
    NativeSocket(int sock);
    ~NativeSocket();
    virtual bool connect(String host, int port);

    virtual bool setBlocking(bool blocking);

    virtual bool send(String *str);

    virtual int receive(char *buf, int bufLen);
};

} // namespace a8::hal::socket::native
