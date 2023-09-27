#pragma once

#include "a8/hal/native/FGSocketReader.h"
#include "a8/util/LineReader.h"
#include "a8/util/SocketReader.h"
#include <string>
using namespace a8::util;
using namespace a8::hal;

namespace a8::hal::native {
class FGSocketLineReader : public FGSocketReader {

private:
    SocketReader *sReader;
    LineReader *lReader;
    int Col_Attitude = -1;

public:
    FGSocketLineReader(Sockets *socketFactory) : FGSocketReader(socketFactory) {
    }
    /**
     * The first line is like below:
     * <LABELS> ,Time,Aileron Command,Elevator Command,Rudder Command,Flap Command,Left Aileron Position,Right Aileron Position,Elevator Position,Rudder Position,Flap Position ?
     */
    virtual void setup(Context &context) override {
        log(">>FGSocketLineReader.setUp()");
        FGSocketReader::setup(context);
        if (context.isStop()) {
            log("Stop context<<FGSocketLineReader.setUp()");
            return;
        }
        sReader = new SocketReader(sockets, sockIn);
        lReader = new LineReader(sReader);
        String headerLine;
        lReader->readLine(headerLine);

        log(String::format("headerline:%s", headerLine.getText()));

        Buffer<String> headers = headerLine.split(',');
        Col_Attitude = headers.indexOf("Attitude");
        log(String::format("<<FGSocketLineReader.setUp(),Col_Attitude:%i", Col_Attitude));
        if (Col_Attitude == -1) {
            context.stop("Column index of \'Attitude\' is -1.");
        }
    }

    virtual void postSetup(Context &context) override {
        FGSocketReader::postSetup(context);
    }

    bool update(SocketData *data) override {

        String line;
        bool hasMore = lReader->readLine(line);
        log(String::format("dataline:%s", line.getText()));
        Buffer<String> fields = line.split(',');
        data->altitude = readDouble(fields, Col_Attitude);
        return hasMore;
    }

    double readDouble(Buffer<String> &fields, int idx) {

        String str = fields.get(idx);
        double ret = std::atof(str.getText());
        return ret;
    }
    float readFloat(Buffer<String> &fields, int idx) {
        return readDouble(fields, idx);
    }
    long readLong(Buffer<String> &fields, int idx) {
        String str = fields.get(idx);
        double ret = std::atol(str.getText());
        return ret;
    }
};

} // namespace a8::hal::native