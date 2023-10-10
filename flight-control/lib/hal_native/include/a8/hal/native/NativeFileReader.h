#pragma once
#include "a8/util/Reader.h"

using namespace a8::util;

namespace a8::hal::native {
class NativeFileReader : public Reader {
private:
    String fpath;
    void * fobj;

public:
    NativeFileReader(String fpath);
    ~NativeFileReader();

    virtual int read(char *buf, int len) override;
};

} // namespace a8::hal::native
