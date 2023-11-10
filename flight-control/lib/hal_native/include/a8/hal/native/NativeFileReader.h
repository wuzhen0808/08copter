#pragma once
#include "a8/util/Reader.h"

namespace a8::hal::native {
using namespace a8::util;

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
