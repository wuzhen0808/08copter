#pragma once
#include "a8/util/Reader.h"
#include <fstream>

using namespace a8::util;

namespace a8::hal::native {
class NativeFileReader : public Reader {
private:
    String fpath;
    std::ifstream is;

public:
    NativeFileReader(String fpath) {
        this->fpath = fpath;
        is.open(fpath.getText(), std::ios::in);
    }
    ~NativeFileReader() {
        is.close();
    }

    virtual int read(char *buf, int len) override {
        int i = 0;

        for (; i < len && is.good(); i++) {
            is.get(*(buf + i));
        }

        if (i == len || i < len && is.eof()) {
            return i;
        }
        
        return -1;
    }
};

} // namespace a8::hal::native
