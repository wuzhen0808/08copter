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
     * Return :
     * >0: the length read into buf parameter provided.
     * 0: no more data to read.
     * <0: error code.
     *
     */
    virtual int read(char *buf, int bufLen) = 0;
    int read(char *buf, int bufLen, bool full) {
        if (!full) {
            return read(buf, bufLen);
        }

        int len = 0;
        while (len < bufLen) {

            int ret = read(buf + len, bufLen - len);
            if (ret < 0) {
                return ret;
            }
            if (ret == 0) {
                break;
            }
            len += ret;
        }
        return len;
    }
    template <typename T>
    int read(T &i) {
        char ch;
        int ret = read(&ch, 1);
        if (ret < 0) {
            return ret;
        }
        i = (T)ch;
        return ret;
    }

    int read(char &ch) {
        return read(&ch, 1);
    }
};

} // namespace a8::util