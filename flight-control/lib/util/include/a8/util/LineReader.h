#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"
#include "a8/util/String.h"

namespace a8::util {
/**
 */
class LineReader {
private:
    const static int BUF_LEN = 128;
    Reader *reader;
    char buf[BUF_LEN];
    int localPointer = -1;
    int localResult;
    int globalResult;

public:
    LineReader(Reader *reader) {
        this->reader = reader;
    }

    ~LineReader() {}

    int readLine(String &ret) {
        return readLine(ret, true);
    }

    int readLine(String &ret, bool appendSeparator) {
        Buffer<char> buf;
        int rst = readLine(buf, appendSeparator);
        ret = buf;
        return rst;
    }

    void syncLocalIfNeeded() {
        if (this->localPointer >= 0) { // Read global at least once.
            if (localResult > 0) {     // has local content waiting to be processed.
                return;
            }

            // has no local content
            // then trying to read from global.
            if (globalResult <= 0) {
                // align with global.
                localResult = globalResult;
                return;
            }

            // globally has more content to be read.
            // locally buffer is empty.
        }
        //
        localResult = globalResult = reader->read(buf, BUF_LEN);
        localPointer = 0;
    }

    int readLine(Buffer<char> &line, bool appendSeparator) {
        int thisResult = 0;
        bool found = false;
        while (true) {
            syncLocalIfNeeded();
            if (localResult <= 0) {
                // align and sync this result with local
                if (thisResult == 0) {
                    // nothing read from this calling so return the local result as error or EOF.
                    thisResult = localResult;
                }
                break;
            }

            // read from local,
            // local result > 0, it's the length of the local buffer.
            // find the line end char.

            int len = localResult;
            for (int i = 0; i < localResult; i++) {

                if (buf[localPointer + i] == '\n') {
                    len = i + 1;
                    found = true;
                    break;
                }
            }
            int len2 = len;
            if (found && !appendSeparator) {
                len2 -= 1;
            }
            line.append(buf, localPointer, len2); //

            localPointer += len;
            thisResult += len;
            localResult -= len;
            if (found) {
                // return thisResult as the length of all appended in this method.
                break;
            }
        }

        return thisResult;
    }
};

} // namespace a8::util