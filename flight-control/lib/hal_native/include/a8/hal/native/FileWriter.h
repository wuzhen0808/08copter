#pragma once
#include "a8/util/String.h"
#include "a8/util/Writer.h"

namespace a8::hal::native {
using namespace a8::util;

class FileWriter : public Writer {
private:
    String file;
    void *fObj;

public:
    FileWriter(String file);

    virtual int write(const char *buf, int bufLen) override;
};
} // namespace a8::hal::native
