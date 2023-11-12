#pragma once
#include "a8/util.h"
#include "a8/util/net/Bridge.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"
#include "a8/util/schedule.h"

using stubCreate = void *(*)(a8::util::net::Channel *);
using anyRelease = void (*)(void *);

namespace a8::util::net {

using namespace a8::util::schedule;

enum Status {
    Idle,
    Bond,
    Listening
};

class Address : public FlyWeight {
private:
    Sockets *sockets;
    Codec *codec;
    String host;
    int port;
    Scheduler *scheduler;
    LoggerFactory *loggerFactory;

    SOCK sock;
    Status status;
    bridge bridge_;

public:
    /**
     * After the address is connected as the client side code.
     * Or after the address is listening new connection as the server side code.
     *
     */

    Address(Sockets *sockets, Codec *codec, bridge bridge, String host, int port, Scheduler *scheduler, LoggerFactory *loggerFactory) : FlyWeight(loggerFactory) {
        this->sockets = sockets;
        this->codec = codec;
        this->host = host;
        this->port = port;
        this->bridge_ = bridge;
        this->status = Idle;
        this->scheduler = scheduler;
        this->loggerFactory = loggerFactory;
    }

    void close() {
        this->sockets->close(this->sock);
    }

    int bind(Result &errorMessage) {
        if (this->status != Idle) {
            return -1; // cannot bind un less Idle.
        }

        SOCK sock;
        int rst = this->sockets->socket(sock);
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

    template <typename T>
    int connect(Bridge<T> *&bridge, T *skeleton, Result &res) {

        int ret = connect<T, T *>(
            bridge, skeleton, [](T *skeleton) { return skeleton; }, res);
        if (ret < 0) {
            Lang::free<T>(skeleton);
        }
        return ret;
    }
    /**
     * Double ways, sending and receiving.
     */
    template <typename T, typename C>
    int connect(Bridge<T> *&bridge, C context, T *(*skeletonCreate)(C), Result &res) {
        if (this->status != Idle) {
            res << "illegal status:" << this->status << ",expected:" << Idle;
            return -1;
        }
        SOCK sock;
        int ret = this->sockets->socket(sock);
        if (ret < 0) {
            this->sockets->close(sock);
            return ret;
        }
        log(">>...connect");
        int rst = this->sockets->connect(sock, this->host, this->port, res);
        log("<<connect...");
        if (rst < 0) {
            log(">>close socket");
            this->sockets->close(sock);
            log("<<close socket");
            res.errorMessage << "Cannot connect to address:" << this->host << ":" << this->port << "\n";
            return rst;
        }

        // client mode.
        Channel *channel = new Channel(sockets, sock, codec);

        bridge = createBridge<T>(context, skeletonCreate, channel);

        return rst;
    }

    template <typename T, typename C>
    Bridge<T> *createBridge(C context, T *(*skeletonCreate)(C), Channel *channel) {
        log(">>createBridge");
        T *skeleton = skeletonCreate(context);
        Bridge<T> *bridge = new Bridge<T>(bridge_, skeleton, channel, loggerFactory);        
        void (*tickF)(void *) = [](void *bridge) {
            static_cast<Bridge<T> *>(bridge)->run();
        };
        log("createBridge.1");
        log("createBridge.3");
        scheduler->schedule(
            tickF, //
            bridge //
        );
        log("<<createBridge");
        return bridge;
    }

    /**
     * Blocking until new connection come in.
     * TODO timeout arg.
     */
    template <typename T>
    int accept(Bridge<T> *&bridge, T *skeleton, Result &rst) {
        int ret = accept<T, T *>(
            bridge, skeleton, [](T *skeleton_) { return skeleton_; }, rst);
        if (ret < 0) {
            Lang::free<T>(skeleton);
        }
        return ret;
    }

    template <typename T, typename C>
    int accept(Bridge<T> *&bridge, C context, T *(*skeletonCreate)(C), Result &rst) {

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
        Channel *channel = new Channel(sockets, sock2, codec);

        bridge = createBridge<T>(context, skeletonCreate, channel);
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
