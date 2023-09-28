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
        for (int i = 0; i < len; i++) {
            if (is.good()) {
                is.get(*(buf + i));
                continue;
            }

            if (i == 0) {
                break;
            } else {
                return i;
            }
        }

        if (is.eof()) {
            return 0;
        }
        return -1;
    }
};

} // namespace a8::hal::native
