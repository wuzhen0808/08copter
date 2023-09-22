#include "a8/hal/Socket.h"
#include "a8/util/Result.h"
namespace a8::hal::native::socket {
using a8::hal::socket::Socket;
using a8::hal::socket::SocketFactory;
using a8::util::Result;
using a8::util::String;

class NativeSocket : public Socket {
private:
    int sock;

public:
    NativeSocket(int sock);
    ~NativeSocket();
    virtual Result connect(String host, int port);

    virtual bool setBlocking(bool blocking);

    virtual bool send(String *str);

    virtual int receive(char *buf, int bufLen);

    virtual int getLastError();
};

class NativeSocketFactory : public SocketFactory {
private:
    int startUpError;

public:
    NativeSocketFactory();
    ~NativeSocketFactory();
    bool isReady();
    virtual Socket *socket();
};
} // namespace a8::hal::native::socket
