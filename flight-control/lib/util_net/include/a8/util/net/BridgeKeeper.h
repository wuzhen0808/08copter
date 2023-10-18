#pragma once

#include "a8/util.h"
#include "a8/util/net/Address.h"
#include "a8/util/net/Bridge.h"

namespace a8::util::net {

template <typename T, typename B>
class BridgeKeeper {

    Bridge<T> *bridge_;
    Address *address;
    bool running;
    EventCenter *eventCenter;
    int eventTypeOfAfterBridgeCreate_;
    int eventTypeOfBeforeBridgeFree_;

public:
    BridgeKeeper(Address *address) {
        this->address = address;
        this->bridge_ = 0;
        this->running = false;
        this->eventCenter = 0;
    }

    void setEventCenter(EventCenter *ec) {
        this->eventCenter = ec;
        this->eventTypeOfAfterBridgeCreate_ = 0;
        this->eventTypeOfBeforeBridgeFree_ = 0;
    }

    ~BridgeKeeper() {
        free(this->bridge_);
    }

    void setEventTypeOfAfterBridgeCreate(int type) {
        eventTypeOfAfterBridgeCreate_ = type;
    }

    void setEventTypeOfBeforeBridgeFree(int type) {
        eventTypeOfBeforeBridgeFree_ = type;
    }
    void notifyEvent(int type, void *data) {
        if (this->eventCenter == 0 || type == 0) {
            return;
        }
        this->eventCenter->notifyEvent(type, data);
    }

    /**
     * client mode,
     */
    template <typename C>
    int get(Bridge<T> *&bridge, T *(*skeletonCreate)(C), B *(*stubCreate)(Channel *), C context, Result &rst) {
        if (bridge_ != 0) { // check if bridge is broken.

            int ret2 = bridge_->isRunning(rst);
            if (ret2 < 0) {
                free(this->bridge_);
            }
        }

        int ret = 0;
        if (bridge_ == 0) { // connect and create a new bridge.
            ret = address->connect<T, C>(bridge_, context, skeletonCreate, rst);
            if (ret < 0) {
                // fail to create the bridge.
                return ret;
            }
            // create stub for this bridge.
            bridge_->template createStub<B>(stubCreate, Lang::free<B>);

            notifyEvent(eventTypeOfAfterBridgeCreate_, bridge_);
        }
        bridge = this->bridge_; // write back ref.
        return ret;
    }
    void free(Bridge<T> *&bridge) {
        if (bridge == 0) {
            return;
        }

        bridge->close();
        notifyEvent(this->eventTypeOfBeforeBridgeFree_, bridge);
        Lang::free<Bridge<T>>(bridge);
        bridge = 0;
    }
    void close() {
        free(this->bridge_);
        this->running = false; // is sync necessary? aka to wait the thread exiting.
        this->address->close();
        //
    }

    int bindAndListen(Result rst) {
        int ret = this->address->bind(rst);
        if (ret < 0) {
            return ret;
        }
        ret = address->listen(rst);
        if (ret < 0) {
            return ret;
        }
        return ret;
    }
    /**
     * Server side mode, accept connection and create bridge, schedule bridge.
     */
    template <typename C>
    int run(T *(*skeletonCreate)(C), B *(*stubCreate)(Channel *), C context, Result rst) {
        int ret = -1; // unknow error.
        Bridge<T> *bridge = 0;
        this->running = true;
        while (this->running) {
            Bridge<T> *bridge2 = 0;
            ret = address->accept<T>(bridge2, context, skeletonCreate, rst);
            if (ret < 0) {
                break;
            }

            bridge2->template createStub<B>(stubCreate, Lang::free<B>);

            free(bridge);
            bridge = bridge2;
            notifyEvent(eventTypeOfAfterBridgeCreate_, bridge);
        }

        // loop exiting, clean the last bridge.
        free(bridge);

        return ret;
    }
};
} // namespace a8::util::net