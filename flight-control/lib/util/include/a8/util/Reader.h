#pragma once
#include "a8/util/ErrorListener.h"

namespace a8::util {
/**
 */
class Reader {
protected:
    ErrorListener *errorListener;

public:
    Reader() {
    }
    Reader(ErrorListener *el) {
        this->errorListener = el;
    }

    ~Reader() {
    }

    virtual int read(char *buf, int bufLen) = 0;
};

} // namespace a8::util