
#include "a8/util/net.h"

using namespace a8::util;
using namespace a8::util::net;

namespace a8::hal::native {

class NativeSockets : public Sockets {

    int startUpError;

public:
    NativeSockets();
    ~NativeSockets();
    bool isReady();
    void close(SOCK &sock) override;

    int connect(SOCK sock, const String host, int port, Result &res) override;

    int bind(SOCK sock, const String address, int port, Result &res) override;

    int listen(SOCK sock, Result &rst) override;

    int accept(SOCK sock, SOCK &sockIn) override;

    bool send(SOCK sock, const char *buf, int len) override;

    int receive(SOCK sock, char *buf, int bufLen) override;

    int getLastError() override;

    int socket(SOCK &sock) override;

    int socket(SOCK &sock, Result &res) override;

    int select(SOCK &sock) override;
    int select(Buffer<SOCK> &buffer1, Buffer<SOCK> &buffer2, Buffer<SOCK> &buffer3) override;
};
} // namespace a8::hal::native
