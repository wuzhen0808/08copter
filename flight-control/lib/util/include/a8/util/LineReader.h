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

protected:
    Reader *reader;
    char buf[BUF_LEN];
    int len = 0;
    int from = -1;
    
public:
    LineReader(Reader *reader) {
        this->reader = reader;
    }

    ~LineReader() {}
    String readLine() {
    }
    void readToBuff() {
        len = reader->read(buf, BUF_LEN);
        from = 0;
    }

    bool readLine(Buffer<char> &buffer) {

        Buffer<char> line;

        if (from == -1) {
            readToBuff();
        }

        while (true) {
            if (this->len <= 0) {
                break;
            }
            int from2 = from;

            for (int i = 0; i < len; i++) {
                from2++;
                if (buf[from2] == '\n') {
                    break;
                }
            }
            int len2 = from2 - from;
            line.append(buf, from, len2);

            if (len2 < len) {
                // skip the '\n'
                from = from2 + 1;
                len = len - len2 - 1;
                break;
            }

            // contains no '\n', read more
            readToBuff();
        }
        buffer.append(line);

        return this->len > 0;
    }
};

} // namespace a8::util