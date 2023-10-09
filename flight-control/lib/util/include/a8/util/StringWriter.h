#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

namespace a8::util {

/**
 */
class StringWriter : public Writer {
protected:
    String str;
    int from = 0;

public:
    StringWriter() {
    }

    ~StringWriter() {
    }
    /**
     * The length of the string may not expected as the buf may contains the end of string character '\0'.
     */
    virtual int write(const char *buf, int bufLen) override {
        str.append(buf, bufLen);
        return bufLen;
    };
    
    String toString() {
        // TODO how to avoid copy.
        return str;
    }
};

} // namespace a8::util