#pragma once
#include "a8/util.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {
enum Status {
    Idle,
    Bond,
    Listening

};

class Address {
private:
    Sockets *sockets;
    Codec *codec;
    String host;
    int port;

    SOCK sock;
    Status status;
    bridge bridge_;

public:
    /**
     * After the address is connected as the client side code.
     * Or after the address is listening new connection as the server side code.
     *
     */

    Address(Sockets *sockets, Codec *codec, bridge bridge, String host, int port) {
        this->sockets = sockets;
        this->codec = codec;
        this->host = host;
        this->port = port;
        this->bridge_ = bridge;
        this->status = Idle;
    }

    int bind(Result &errorMessage) {
        if (this->status != Idle) {
            return -1; // cannot bind un less Idle.
        }

        SOCK sock;
        int rst = this->sockets->socket(sock, errorMessage);
        if (rst < 0) {
            return rst;
        }

        int ret = this->sockets->bind(sock, this->host, this->port, errorMessage); //
        if (ret < 0) {
            this->sockets->close(sock);
            return ret;
        }
        this->sock = sock;
        this->status = Bond;
        return ret;
    }
    /**
     * Register the listener on address.
     */
    int listen(Result &rst) {

        if (this->status != Bond) {
            rst.errorMessage << "cannot listen on address(" << *this << ") before bond.";
            return -1; // not bind yet.
        }
        int ret = this->sockets->listen(this->sock, rst);
        if (ret < 0) {
            return ret;
        }
        this->status = Listening;
        return ret;
    }
    /**
     * Double ways, sending and receiving.
     */
    template<typename K, typename S>
    int connect(Channel<K, S> *&channel, K *skeleton, S *(*stubCreate)(Channel<K, S> *), void (*stubRelease)(S *), Result &res) {
        if (this->status != Idle) {
            return -1;
        }
        SOCK sock;
        int ret = this->sockets->socket(sock, res);
        if (ret < 0) {
            this->sockets->close(sock);
            return ret;
        }

        int rst = this->sockets->connect(sock, this->host, this->port, res);
        if (rst < 0) {
            this->sockets->close(sock);
            res.errorMessage << "Cannot connect to address:" << this->host << ":" << this->port << "\n";
            return rst;
        }
        // client mode.
        channel = new Channel<K, S>(sockets, sock, codec, bridge_, skeleton, stubCreate, stubRelease);
        return rst;
    }

    /**
     * Blocking until new connection come in.
     */
    template <typename K, typename S>
    int accept(Channel<K, S> *&channel, K *skeleton, S *(*stubCreate)(Channel<K, S> *), void (*stubRelease)(S *), Result &rst) {

        if (this->status != Listening) {
            rst.errorMessage << "Could not accept connection on address" << this << ",not listening yet.";
            return -1; // not listening yet.
        }

        SOCK sock2;
        int ret = sockets->accept(this->sock, sock2);
        if (ret < 0) {
            this->sockets->close(this->sock);
            this->sock = 0;
            this->status = Idle;
            rst.errorMessage << "cannot accept socket on address:" << this->host << ":" << this->port;
            return ret;
        }
        channel = new Channel<K, S>(sockets, sock2, codec, bridge_, skeleton, stubCreate, stubRelease);
        return ret;
    }

    friend String &operator<<(String &str, Address *address) {
        return operator<<(str, *address);
    }

    friend String &operator<<(String &str, Address &address) {
        return str << address.host << ":" << address.port;
    }
};

} // namespace a8::util::net
