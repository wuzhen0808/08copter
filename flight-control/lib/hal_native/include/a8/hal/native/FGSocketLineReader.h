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

    int Col_Time;     // Time
    int Col_Altitude; //
    int Col_Phi;
    int Col_Tht;
    int Col_Psi;
    int Col_Alpha;
    int Col_Beta;
    int Col_Latitude;
    int Col_Longitude;

public:
    FGSocketLineReader(Sockets *socketFactory) : FGSocketReader(socketFactory) {
    }
    /**
     * The header and data lines are like below:
     *
       <LABELS> ,Time,Altitude,Phi (deg),Tht (deg),Psi (deg),Alpha (deg),Beta (deg),Latitude (deg),Longitude (deg)
       89.85,    5777.205,   0.8062249, -0.02998105,9.955398e-05,   -89.90706,    2.471205,    30.83043,    121.0004
     */
    virtual void setup(Context *context) override {
        log(">>FGSocketLineReader.setUp()");
        FGSocketReader::setup(context);
        if (context->isStop()) {
            log("Stop context<<FGSocketLineReader.setUp()");
            return;
        }
        sReader = new SocketReader(sockets, sockIn);
        lReader = new LineReader(sReader);
        String headerLine;
        lReader->readLine(headerLine, false);

        log(String::format("headerline:'%s',len:%i", headerLine.getText(), headerLine.getLength()));

        Buffer<String> headers = headerLine.split(',');

        String lastCol = headers.get(headers.getLength() - 1);
        log(String::format("headers.len:'%i', last col:'%s' ", headers.getLength(), lastCol));

        Col_Time = findIndex(context, headers, "Time");
        Col_Altitude = findIndex(context, headers, "Altitude"); //
        Col_Phi = findIndex(context, headers, "Phi (deg)");
        Col_Tht = findIndex(context, headers, "Tht (deg)");
        Col_Psi = findIndex(context, headers, "Psi (deg)");
        Col_Alpha = findIndex(context, headers, "Alpha (deg)");
        Col_Beta = findIndex(context, headers, "Beta (deg)");
        Col_Latitude = findIndex(context, headers, "Latitude (deg)");
        Col_Longitude = findIndex(context, headers, "Longitude (deg)");
    }

    int findIndex(Context *context, Buffer<String> &headers, String name) {
        int idx = headers.indexOf(name);
        if (idx < 0) {
            context->stop("No index found from header of the JSBSim socket data output for column:" + name);
        }
        return idx - 1;
    }

    virtual void postSetup(Context *context) override {
        FGSocketReader::postSetup(context);
    }

    bool update(SocketData *data) override {

        String line;
        int read = lReader->readLine(line);
        if (read <= 0) {            
            return false;
        }

        log(String::format("dataline:%s", line.getText()));
        Buffer<String> fields = line.split(',');
        data->altitude = readDouble(fields, Col_Altitude);

        return true;
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