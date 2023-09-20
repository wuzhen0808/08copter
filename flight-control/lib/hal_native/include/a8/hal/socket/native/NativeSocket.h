#include "a8/hal/socket.h"
namespace a8::hal::socket::native {
using a8::hal::socket::Connection;

class NativeConnection : public Connection {
private:
    int sock;
    int clientFileDescriptor;
    

public:
    virtual NativeConnection *connect(String host, int port);
    
    virtual void close();

    virtual int send(const char *buf, int size);

    virtual void receive();
};

} // namespace a8::hal::socket::native
