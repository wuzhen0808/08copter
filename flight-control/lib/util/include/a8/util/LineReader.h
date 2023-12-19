#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/Reader.h"
#include "a8/util/Result.h"
#include "a8/util/String.h"

namespace a8::util {
/**
 */
class LineReader : public Reader {
private:
    const static int BUF_LEN = 128;
    // dynamic member
    Reader *reader;
    char buf[BUF_LEN];
    int localPointer;
    int localResult;
    int globalResult;

    void syncLocalIfNeeded(Result &res) {
        if (this->localPointer != -1) { // Read global at least once.
            if (localResult > 0) {      // has local content waiting to be processed.
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
        localResult = globalResult = reader->read(buf, BUF_LEN, res);
        localPointer = 0;
    }

public:
    LineReader(Reader *reader) {
        this->setReader(reader);
    }

    ~LineReader() {}

    void setReader(Reader *reader) {
        this->reader = reader;
        this->localPointer = -1; // indicated as not reading
    }

    int read(char *buf, int bufLen, Result &res) override {

        syncLocalIfNeeded(res);
        if (localResult <= 0) {
            return localResult;
        }
        int len = Math::min(bufLen, localResult);
        Lang::copy<char>(this->buf, this->localPointer, len, buf);
        this->localPointer += len;
        this->localResult -= len;
        return len;
    }

    /**
     * Read one line and append to the ret parameter ref.
     *
     * Return:
     * The total len read, including the line break char.
     *
     * - 0: EOF
     * - >0: the length of the line read, including the line break char if not end.
     * - <0: some error.
     *
     */
    int readLine(String &ret) {
        Result rst;
        return readLine(ret, true, rst);
    }

    int readLine(String &ret, Result &rst) {
        return readLine(ret, true, rst);
    }

    /**
     * Parmeters:
     * - line: the reference which the line is append to.
     * - appendSeparator:
     * -- true: append the ling break char to the result string.
     * -- false: drop the ling break char and return the line with out the line break.
     * Return:
     * - 0, EOF
     * - >0， length of the chars read.
     * - <0,  some error.
     */
    int readLine(String &line, bool appendSeparator) {
        Result rst;
        return readLine(line, appendSeparator, rst);
    }

    int readLine(String &line, bool appendSeparator, Result &res) {
        return readLine<int>(
            line, appendSeparator, 0, [](int, char ch) { return ch == '\n'; }, res);
    }

    template <typename C>
    int readLine(String &line, bool appendSeparator, C c, bool (*isLineBreak)(C, char), Result &res) {
        int thisResult = 0;
        bool found = false;
        while (true) {
            syncLocalIfNeeded(res);
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

                if (isLineBreak(c, buf[localPointer + i])) {
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