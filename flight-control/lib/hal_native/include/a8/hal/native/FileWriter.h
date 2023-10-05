#pragma once
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include <fstream>
#include <iostream>

using namespace a8::util;

namespace a8::hal::native {
class FileWriter : public Writer {
private:
    std::ofstream ofstream;
    String file;
public:
    FileWriter(String file) {
        this->file = file;
    }

    virtual int write(const char *buf, int bufLen) override {
        if (!ofstream.is_open()) {
            ofstream.open(file.text(), std::ios::out);            
        }

        ofstream.write(buf, bufLen);
        ofstream.flush();
        return bufLen;
    }
};
} // namespace a8::hal::native
