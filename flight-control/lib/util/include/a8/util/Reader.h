#pragma once
#include "a8/util/ErrorListener.h"
#include "a8/util/Result.h"

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

    int fullRead(char *buf, int bufLen) {
        Result res;
        return fullRead(buf, bufLen, res);
    }

    int fullRead(char *buf, int bufLen, Result &res) {
        int len = 0;
        while (len < bufLen) {
            int len2 = bufLen - len;
            int ret = read(buf + len, len2, res);
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
    int read(char *buf, int bufLen) {
        Result res;
        return read(buf, bufLen, res);
    }
    
    /**
     * Return :
     * >0: the length read into buf parameter provided.
     * 0: no more data to read.
     * <0: error code.
     *
     */
    virtual int read(char *buf, int bufLen, Result &res) = 0;

    int read(char *buf, int bufLen, bool full, Result &res) {
        if (!full) {
            return read(buf, bufLen, res);
        }

        int len = 0;
        while (len < bufLen) {

            int ret = read(buf + len, bufLen - len, res);
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
        Result res;
        return read(i, res);
    }

    template <typename T>
    int read(T &i, Result &res) {
        char ch;
        int ret = read(&ch, 1, res);
        if (ret < 0) {
            return ret;
        }
        i = (T)ch;
        return ret;
    }
    int read(char &ch) {
        Result res;
        return read(ch, res);
    }
    int read(char &ch, Result &res) {
        return read(&ch, 1, res);
    }
};

} // namespace a8::util