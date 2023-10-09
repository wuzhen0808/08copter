#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/FunctionalCodec.h"

using namespace a8::util;
using namespace a8::util::net;
#define SIMPLE_CODEC_ (0)

namespace a8::util::net {

class SimpleCodec : public Codec {

    // dynamic members.
    Buffer<Codec *> subCodecs;

public:
    SimpleCodec() : Codec() {
    }

    int getHeaderLength() override {
        return 1;
    }
    /**
     * Register a data type with encoder and decoder.
     * And optionally provide a default handle function pointer for receiving the data decoded.
     * If no the handle function pointer not provided, the received data will be ignored.
     */
    void add(int type, Codec *subCodec) {
        while (subCodecs.length() < type + 1) {
            subCodecs.append(static_cast<Codec *>(0));
        }
        if (subCodecs.get(type) != 0) {
            // TODO error process?
        }
        subCodecs.set(type, subCodec);
    }

    template <typename T>
    void add(int type, int (*writeF)(Writer *, T *), int (*readF)(Reader *, T *&)) {
        add<T>(type, writeF, readF, [](T *ptr) { Lang::free(ptr); });
    }

    template <typename T>
    void add(int type, int (*writeF)(Writer *, T *), int (*readF)(Reader *, T *&), void (*freeF)(T *)) {
        add(type, new FunctionalCodec<T>(type, writeF, readF, freeF));
    }

    /**
     * @override
     */
    int write(Writer *writer, int type, void *data, Result &rst) override {

        int len = 0;

        int ret = CodecUtil::writeInt8(writer, type); // sub codec
        if (ret < 0) {
            return ret;
        }
        len += ret;
        Codec *subCodec = subCodecs.get(type);
        if (subCodec != 0) {
            ret = subCodec->write(writer, type, data, rst);
            if (ret < 0) {
                return ret;
            }
            len += ret;
        }
        //
        return len;
    }

    /**
     * @override
     */
    int read(Reader *reader, bridge bridgeF, void *context, Result &rst) override {
        int ret2 = 0;
        int type;
        int ret = CodecUtil::readInt8(reader, type);
        if (ret < 0) {
            return ret;
        }
        ret2 = ret;
        Codec *codec = subCodecs.get(type);
        if (codec == 0) {
            rst.errorMessage << "cannot decode, no codec found for data type:" << type;
            return -2; // failed to decode buffer.
        }

        ret = codec->read(reader, bridgeF, context, rst); // write ref back
        if (ret < 0) {
            return ret;
        }
        ret2 += ret;

        return ret2;
        //
    }
};

} // namespace a8::util::net
