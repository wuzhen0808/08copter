#pragma once
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/Address.h"
#include "a8/util/net/Channel.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"
#include "a8/util/net/defines.h"

namespace a8::util::net {
/**
 *  Network is a utility wrapper on top of socket.
 *
 *  The purpose of the Network class is providing a framework for implementing your protocol by
 *  registering a set of message types.
 *
 *  Each type of message has tree attributes:
 *      1. type id, which is a int value.
 *      2. an encode function pointer.
 *      3. a decode function pointer.
 *
 *  For example the following code initialized a Network with 3 types of message.
 *
 *      Codec *codec = new SimpleCodec();
 *      network = new Network(sockets, codec, 0); //
 *      // register address
 *      this->gsAddress = network->add(host, gsPort);
 *      this->fcAddress = network->add(host, fcsPort);
 *      // register codecs
 *      network->add(CommonMessageType::PING, Functions::encodeString, Functions::decodeString);
 *      network->add(CommonMessageType::LOG, Functions::encodeString, Functions::decodeString);
 *      network->add(CommonMessageType::CMD, Functions::encodeString, Functions::decodeString);
 *
 *  For the code to use this network:
 *      Channel *channel;
 *      int rst = network->connect(fcAddress, channel, 0, 0, errorMessage);
 *      if(rst<0){
 *          //process error.
 *      } else {
 *          String msg = "a message.";
 *          channel->send(CommonMessageType::PING, &msg);
 *          channel->send(CommonMessageType::LOG, &msg);
 *          channel->send(CommonMessageType::CMD, &msg);
 *      }
 *
 */
class Network {
    void *context;
    Codec *codec;
    Sockets *sockets;

    Buffer<Address *> *addresses;

public:
    Network(Sockets *sockets, Codec *codec, void *context) {
        this->context = context;
        this->sockets = sockets;
        this->codec = codec;
        this->addresses = new Buffer<Address *>();
    }
   
    /**
     * Register a data type with encoder and decoder.
     * And optionally provide a default handle function pointer for receiving the data decoded.
     * If no the handle function pointer not provided, the received data will be ignored.
     */
    void add(int type, FuncType::encode encoder, FuncType::decode decoder) {
        this->codec->add(type, encoder, decoder);
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
        addr->listening();
        channel = new Channel(sockets, addr, sock, codec, listen, context);
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
        if (ret > 0) {
            addr->bond(sock);
        }
        return ret;
    }
    /**
     * Register the listener on an address.
     */
    int listen(int adr, Result &rst) {
        Address *address = this->getAddress(adr);
        if (address->status != Bond) {
            rst.errorMessage << "cannot listen on address(" << *address << ") before bond.";
            return -1; // not bind yet.
        }
        int ret = this->sockets->listen(address->sock, rst);
        if (ret < 0) {

            return ret;
        }

        address->listening();

        return ret;
    }
    /**
     * Blocking until new connection come in.
     */
    int accept(int addressId, Channel *&channel, FuncType::handle listen, void *context, Result &rst) {
        Address *addr = this->getAddress(addressId);
        if (addr->status != Listening) {
            rst.errorMessage << "Could not accept connection on address" << addr << ",not listening yet.";
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
        channel = new Channel(sockets, addr, sock2, codec, listen, context);
        return ret;
    }
};

} // namespace a8::util::net
