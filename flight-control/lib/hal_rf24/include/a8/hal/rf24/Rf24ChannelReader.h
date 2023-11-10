#pragma once
#include "a8/hal/rf24/Rf24ChannelsApi.h"
#include "a8/hal/rf24/Rf24NetworkWrapper.h"
#include "a8/util/net.h"

class RF24;
template <class T>
class ESBNetwork;
namespace a8::hal::rf24 {

class Rf24ChannelReader : public Reader {
    Rf24ChannelsApi *channels;
    Queue<char> *queue;
    int channelId;
public:
    Rf24ChannelReader(int channelId,Rf24ChannelsApi *channels, Queue<char> *queue) {
        this->channelId = channelId;
        this->channels = channels;
        this->queue = queue;
    }

    ~Rf24ChannelReader() {
    }

    int read(char *buf, int bufLen) {
        
        int len = this->queue->len();
        if(len == 0){
            
            int len1 = this->channels->readToQueue(channelId);

        }
        if (len > bufLen) {
            len = bufLen;
        }
        for (int i = 0; i < len; i++) {
            char ch;
            queue->take(ch);
            buf[i] = ch;
        }
        return len;
    }
};

} // namespace a8::hal::nrf24