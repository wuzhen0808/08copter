#pragma once
#include "a8/util/Pair.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

using namespace a8::util;

namespace a8::net {

typedef int (*encodeFunc)(void *, char *&);

class Encoder {

    int writeInt8(Writer *writer, int iValue) {
        char ch = iValue & 0x000000FF;
        writer->write(&ch, 1);
        return 1;
    }

    int writeInt16(Writer *writer, int iValue) {
        char ch1 = iValue >> 0 & 0x000000FF;
        char ch2 = iValue >> 8 & 0x000000FF;
        writer->write(&ch1);
        writer->write(&ch2);
        return 2;
    }

    Buffer<encodeFunc> encoders;

public:
    Encoder() {
    }

    void set(int type, encodeFunc encoder) {

        while (encoders.getLength() < type + 1) {
            encoders.append(0);
        }
        if (encoders.get(type) != 0) {
            // TODO error process?
        }
        encoders.set(type, encoder);
    }

    int write(Writer *writer, int type, void *data) {

        writeInt8(writer, 0); // ver
        writeInt8(writer, type);

        encodeFunc encode = encoders.get(type);
        if (encode == 0) {
            // ignore message? or log error.
            return -1;
        }
        char *buf = 0;
        int len = (encode)(data, buf);

        if (len < 0) {
            return -1;
        }
        writer->write(buf, len);
        //
        return len;
    }
};

} // namespace  a8::net
