#pragma once
#include "a8/util/Pair.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

using namespace a8::util;

namespace a8::net {

typedef void *(*parseFunc)(int, char *);
typedef void (*handleFunc)(int, void *);

class Decoder {

    Buffer<parseFunc> parsers;
    Buffer<handleFunc> handlers;

    int readInt8(Reader *reader, char *buf, int len) {
        reader->read(buf, 1);
        int i = 0;
        i = (i + buf[0] & 0x000000FF) << 0;
        return i;
    }

    int readInt16(Reader *reader, char *buf, int len) {
        reader->read(buf, 2);
        int i = 0;
        i = (i + buf[0] & 0x000000FF) << 0;
        i = (i + buf[1] & 0x000000FF) << 8;
        return i;
    }

public:
    Decoder() {
    }

    void set(int type, parseFunc parser, handleFunc handler) {

        while (parsers.getLength() < type + 1) {
            parsers.append(0);
            handlers.append(0);
        }
        if (parsers.get(type) != 0) {
            // TODO error process?
        }
        parsers.set(type, parser);
        handlers.set(type, handler);
    }

    int read(Reader *reader) {
        int len0 = 2;
        char buf[len0];

        int ver = readInt8(reader, buf, len0);
        int type = readInt8(reader, buf, len0);
        int len2 = readInt16(reader, buf, len0);
        char buf2[len2];
        int rst = reader->read(buf2, len2);
        if (rst < len2) {
            // some error or the connection broken.
            return -1;
        }
        parseFunc par = parsers.get(type);
        if (par == 0) {
            return -1;
        }
        void *msg = (parseFunc)(len2, buf2);
        handleFunc handle = handlers.get(type);
        if (handle == 0) {
            return -1;
        }
        (handle)(type, msg);
        return type;
        //
    }
};

} // namespace  a8::net
