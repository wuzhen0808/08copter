#pragma once
#include "a8/hal/rf24/Rf24Channel.h"
#include "a8/hal/rf24/Rf24ChannelData.h"
#include "a8/hal/rf24/Rf24ChannelReader.h"
#include "a8/hal/rf24/Rf24ChannelsApi.h"
#include "a8/hal/rf24/Rf24NetworkWrapper.h"

#include "a8/util/net.h"

namespace a8::hal::rf24 {

class Rf24ChannelsImpl : public Rf24ChannelsApi {

    Rf24NetworkWrapper *network;
    HashTable<int, Rf24Channel *> *channels;
    int nextId;

public:
    Rf24ChannelsImpl() {
        this->channels = new HashTable<int, Rf24Channel *>();
        this->nextId = 1;
    }

    ~Rf24ChannelsImpl() {
        delete this->channels;
    }

    int create(int port1, int port2) {
        int channelId = this->nextId++;
        Queue<char> *queue = new Queue<char>();
        Rf24ChannelReader *reader = new Rf24ChannelReader(channelId, this, queue);
        Rf24Channel *channel = new Rf24Channel(port1, port2, reader, queue);
        channels->set(channelId, channel);
    }

    int readToQueue(int channelId) override {
        int len = 0;
        int ret = 0;
        while (true) {
            int size = this->network->peekSize();
            if (size == 0) {
                break; // no more
            }
            int channelId2;
            ret = doReadToQueue(size, channelId, channelId2);
            if (ret < 0) {
                // TODO save the error code for channel?
                break;
            }

            if (channelId2 == channelId) {
                len += size;
                break;
            }
        }

        return ret;
    }
    int doReadToQueue(int size, int channelId1, int &channelId2) {
        int nodeId1;
        char buf[size];
        this->network->read(nodeId1, buf, size);

        BufferReader br(buf, size);

        Rf24ChannelData data;
        int ret = Rf24ChannelData::read(&br, data);
        if (ret < 0) {
            return ret;
        }
        if (ret == 0) {
            return -1; // impossible,because size > 0
        }

        channelId2 = data.channel; // write back.

        Rf24Channel *channel = channels->get(channelId2, 0);
        if (channel == 0) {
            // ignore?
            if (channelId1 == channelId2) {
                return -1; // bug?
            } else {       //
                // ignore?
            }
        } else {
            Queue<char> *q = channel->getQueue();
            q->append(data.buffer, data.size);
        }

        return ret;
    }
};

} // namespace a8::hal::nrf24