#include "a8/hal/native/FileWriter.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"
#include <fstream>
#include <iostream>

using namespace a8::util;

namespace a8::hal::native {

FileWriter::FileWriter(String file) {
    this->file = file;
    std::ofstream *ofstream = new std::ofstream();
    this->fObj = ofstream;
}

int FileWriter::write(const char *buf, int bufLen) {
    std::ofstream *ofstream = Lang::cast<std::ofstream>(fObj);
    if (!ofstream->is_open()) {
        ofstream->open(file.text(), std::ios::out);
    }

    ofstream->write(buf, bufLen);
    ofstream->flush();
    return bufLen;
}

} // namespace a8::hal::native
