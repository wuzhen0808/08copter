#pragma once
namespace a8::util::rf24 {

class Rf24Hal {
public:
    class Radio {
    public:
        virtual bool begin() = 0;
        virtual void setChannel(int channel) = 0;
    };

    class Network {
    public:
        virtual void begin(int nodeId) = 0;
        virtual int peek() = 0;
        virtual bool available() = 0;
        virtual void update() = 0;
        virtual int read(char *buf, int len) = 0;
        virtual int write(int nodeId, char *buf, int len) = 0;
    };

public:
    virtual Network *newNetwork(Radio *radio) = 0;
    virtual Radio *newRadio(int ce, int cs) = 0;
};
} // namespace a8::util::rf24