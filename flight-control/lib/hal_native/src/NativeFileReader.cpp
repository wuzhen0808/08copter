#include "a8/hal/native/NativeFileReader.h"
#include <fstream>

using namespace a8::util;

namespace a8::hal::native {
NativeFileReader::NativeFileReader(String fpath) {
    this->fpath = fpath;
    std::ifstream *is = new std::ifstream();
    this->fobj = is;
    is->open(fpath.text(), std::ios::in);
}
NativeFileReader::~NativeFileReader() {

    std::ifstream *is = static_cast<std::ifstream *>(fobj);
    is->close();
    delete is;
}

int NativeFileReader::read(char *buf, int len) {
    int i = 0;
    std::ifstream *is = static_cast<std::ifstream *>(fobj);
    for (; i < len && is->good(); i++) {
        is->get(*(buf + i));
    }

    if (i == len || i < len && is->eof()) {
        return i;
    }

    return -1;
}

} // namespace a8::hal::native
