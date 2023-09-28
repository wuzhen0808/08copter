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
    /**
     * return :
     * >0: the length read into buf parameter provided.
     * 0: no more data to read.
     * <0: error code.
     */
    virtual int read(char *buf, int bufLen) = 0;
};

} // namespace a8::util