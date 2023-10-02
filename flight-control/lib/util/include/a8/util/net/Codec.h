#pragma once
#include "a8/util/net/defines.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include "a8/util/net/SocketReader.h"
#include "a8/util/net/SocketWriter.h"
#include "a8/util/net/Sockets.h"

namespace a8::util::net {

struct EnDe {
    FuncType::encode encode;
    FuncType::decode decode;
};

class Codec {
    Buffer<EnDe *> codecs;

public:
    void add(int type, FuncType::encode encoder, FuncType::decode decoder) {
        EnDe *codec = new EnDe();
        codec->encode = encoder;
        codec->decode = decoder;
        while (codecs.getLength() < type + 1) {
            codecs.append(0);
        }
        if (codecs.get(type) != 0) {
            // TODO error process?
        }
        codecs.set(type, codec);
    }

    virtual int write(Writer *writer, int type, void *data) = 0;

    virtual int read(Reader *reader, int &type, void *&data) = 0;
};

} // namespace a8::util::net
