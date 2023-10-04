#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Address.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/Dispatcher.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {

class Network {
    void *context;
    Codec *codec;
    Sockets *sockets;
    Dispatcher *dispatcher;

    Buffer<Address *> *addresses;

public:
    Network(Sockets *sockets, Codec *codec, void *context) {
        this->context = context;
        this->sockets = sockets;
        this->codec = codec;
        this->addresses = new Buffer<Address *>();
        this->dispatcher = new Dispatcher(0);
    }
    void setDefaultHandle(FuncType::handle handle) {
        dispatcher->setDefaultHandle(handle);
    }
    void add(int type, FuncType::encode encoder, FuncType::decode decoder) {
        add(type, encoder, decoder, 0);
    }
    /**
     * Register a data type with encoder and decoder.
     * And optionally provide a default handle function pointer for receiving the data decoded.
     * If no the handle function pointer not provided, the received data will be ignored.
     */
    void add(int type, FuncType::encode encoder, FuncType::decode decoder, FuncType::handle handle) {
        this->codec->add(type, encoder, decoder);
        this->dispatcher->addHandle(type, handle);
    }
    /**
     * Register an address, to track the status for binding and listening.
     */
    int add(String host, int port) { //
        // todo avoid duplicate.
        Address *add = new Address(host, port);
        int ret = this->addresses->length();
        this->addresses->append(add);
        return ret;
    }

    Address *getAddress(int addr) {
        return this->addresses->get(addr);
    }

    /**
     * Single way, sending only.
     */
    int connect(int address, Channel *&channel, Result &errorMessage) {
        return connect(address, channel, 0, 0, errorMessage);
    }
    /**
     * Double ways, sending and receiving.
     */

    int connect(int address, Channel *&channel, FuncType::handle listen, void *context, Result &res) {

        SOCK sock;
        int ret = this->sockets->socket(sock, res);
        if (ret < 0) {
            return ret;
        }

        Address *addr = this->getAddress(address);
        int rst = this->sockets->connect(sock, addr->host, addr->port, res);
        if (rst < 0) {
            this->sockets->close(sock);
            res.errorMessage << "Cannot connect to address:" << addr->host << ":" << addr->port << "\n";
            return rst;
        }
        // client mode.
        addr->listening(listen, context);
        channel = new Channel(sockets, addr, sock, codec, dispatcher);
        return rst;
    }

    int bind(int address, Result &errorMessage) {
        Address *addr = this->getAddress(address);
        if (addr->status != Idle) {
            return -1; // cannot bind un less Idle.
        }

        SOCK sock;
        int rst = this->sockets->socket(sock, errorMessage);
        if (rst < 0) {
            return rst;
        }

        int ret = this->sockets->bind(sock, addr->host, addr->port, errorMessage); //
        if (ret == 0) {
            addr->bond(sock);
        }
        return ret;
    }
    /**
     * Register the listener on an address.
     */
    int listen(int adr, FuncType::handle listen, void *context, Result &rst) {
        Address *address = this->getAddress(adr);
        if (address->status != Bond) {
            rst.errorMessage << "cannot listen on address" << *address << "before bond.";
            return -1; // not bind yet.
        }
        int ret = this->sockets->listen(address->sock, rst);
        if (ret < 0) {

            return ret;
        }

        address->listening(listen, context);

        return ret;
    }
    /**
     * Blocking until new connection come in.
     */
    int accept(int addressId, Channel *&channel, Result &rst) {
        Address *addr = this->getAddress(addressId);
        if (addr->status != Listening) {
            rst.errorMessage << "Not listening yet.";
            return -1; // not listening yet.
        }

        SOCK sock2;
        int ret = sockets->accept(addr->sock, sock2);
        if (ret < 0) {
            this->sockets->close(addr->sock);
            addr->sock = 0;
            addr->status = Idle;
            rst.errorMessage << "cannot accept socket on address:" << addr->host << ":" << addr->port;
            return ret;
        }
        channel = new Channel(sockets, addr, sock2, codec, dispatcher);
        return ret;
    }
};

} // namespace a8::util::net
