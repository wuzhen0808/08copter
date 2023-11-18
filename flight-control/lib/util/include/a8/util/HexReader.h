#pragma once
#include "a8/util/Lang.h"
#include "a8/util/Reader.h"

namespace a8::util {

/**
 */
class HexReader : public Reader {

    Reader *reader;
    char nextHex = '\0';

public:
    HexReader(Reader *reader) {
        this->reader = reader;
    }

    int read(char *buf, int bufLen, Result &res) override {

        if (nextHex != '\0') {
            buf[0] = nextHex;
            nextHex = '\0';
            return 1;
        }

        char ch;
        int ret = reader->read(&ch, 1, res);
        if (ret < 0) {
            return ret;
        }
        if (ret == 0) { // EOF
            return 0;
        }

        buf[0] = Lang::high4Hex(ch);
        this->nextHex = Lang::low4Hex(ch);
        return 1;
    };

};

} // namespace a8::util