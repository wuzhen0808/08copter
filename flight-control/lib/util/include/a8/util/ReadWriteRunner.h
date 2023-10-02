#pragma once
#include "a8/util/ErrorListener.h"
#include "a8/util/Logger.h"
#include "a8/util/Reader.h"
#include "a8/util/Result.h"
#include "a8/util/Runnable.h"
#include "a8/util/String.h"
#include "a8/util/Writer.h"

using namespace a8::util;
namespace a8::util {

class ReadWriteRunner : public Runnable {
private:
    Reader *reader;
    Writer *writer;
    bool running = true;
    Result state;

public:
    ReadWriteRunner(Reader *reader, Writer *writer) {
        this->reader = reader;
        this->writer = writer;
    }
    ~ReadWriteRunner() {}

    virtual void run() {
        while (running) {
            char buf[100];
            int len = reader->read(buf, 100);
            if (len <= 0) {
                if (len < 0) { // error
                    state.errorMessage << "Failed to read, stop receiving.";

                    break; // end loop
                }
            }
            writer->write(buf, len);
        }
    };
};
} // namespace a8::util