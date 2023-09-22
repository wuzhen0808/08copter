#include "a8/util/Result.h"
#include "a8/util/Socket.h"

using namespace a8::util;

namespace a8::hal::native {

class NativeSocket : virtual public Socket {

protected:
    int sock;

public:
    NativeSocket(int sock);
    ~NativeSocket();

    virtual bool setBlocking(bool blocking) override;

    virtual bool send(String *str) override;

    virtual int receive(char *buf, int bufLen) override;

    virtual int getLastError() override;
};

class NativeSocketClient : public NativeSocket, public SocketClient {
public:
    NativeSocketClient(int sock) : NativeSocket(sock) {
    }
    virtual Result connect(String host, int port) override;
};

class NativeSocketServer : public NativeSocket, public SocketServer {
public:
    NativeSocketServer(int sock) : NativeSocket(sock) {
    }
    virtual Result bind(String host, int port) override;
    virtual Result listen() override;
    virtual Socket *accept() override;
};

class NativeSocketFactory : public SocketFactory {
private:
    int startUpError;
    int socket();

public:
    NativeSocketFactory();
    ~NativeSocketFactory();
    bool isReady();
    SocketClient *socketClient() override;
    SocketServer *socketServer() override;
};
} // namespace a8::hal::native
