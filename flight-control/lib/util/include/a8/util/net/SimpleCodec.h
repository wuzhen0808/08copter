#pragma once
#include "a8/util.h"
#include "a8/util/net/Codec.h"
#include "a8/util/net/FunctionCodec.h"

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

    void add(int type, FuncType::write write, FuncType::read read) {
        add(type, new FunctionCodec(write, read));
    }

    /**
     * @override
     */
    int write(Writer *writer, int type, void *data, Result &rst) override {

        int len = 0;

        int ret = CodecFunc::writeInt8_(writer, type); // sub codec
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
    int read(Reader *reader, int &type, void *&data, Result &rst) override {
        int ret2 = 0;
        int ret = CodecFunc::readInt8_(reader, type);
        if (ret < 0) {
            return ret;
        }
        ret2 = ret;
        Codec *codec = subCodecs.get(type);
        if (codec == 0) {
            data = 0; // write ref back
            rst.errorMessage << "cannot decode, no codec found for data type:" << type;
            return -2; // failed to decode buffer.
        }

        ret = codec->read(reader, type, data, rst); // write ref back
        if (ret < 0) {
            return ret;
        }
        ret2 += ret;
        return ret2;
        //
    }
};

} // namespace a8::util::net
