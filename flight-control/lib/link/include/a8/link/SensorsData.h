#pragma once

#include "a8/util.h"
#include "a8/util/net.h"

namespace a8::link {
using namespace a8::util;


// Stub to ground station interface.
struct SensorsData {

    static void default_(SensorsData &data) {
    }

    static int write(Writer *writer, SensorsData data) {
        int ret = 0;
        ret += CodecUtil::writeFloat64(writer, data.altitude);
        ret += CodecUtil::writeFloat64(writer, data.latitude);
        ret += CodecUtil::writeFloat64(writer, data.longitude);
        ret += Vector3f::write(writer, data.angVel);
        ret += Vector3f::write(writer, data.accVel);
        ret += CodecUtil::writeString(writer, data.errorMessage);
        return ret;
    }

    static int read(Reader *reader, SensorsData &data) {
        int ret = 0;
        ret += CodecUtil::readFloat64(reader, data.altitude);
        ret += CodecUtil::readFloat64(reader, data.latitude);
        ret += CodecUtil::readFloat64(reader, data.longitude);
        ret += Vector3f::read(reader, data.angVel);
        ret += Vector3f::read(reader, data.accVel);
        ret += CodecUtil::readString(reader, data.errorMessage);
        return ret;
    }

    double altitude;
    double latitude;
    double longitude;
    Vector3f angVel; // angular velocity
    Vector3f accVel; // acceleration velocity
    String errorMessage;
    SensorsData() {
        this->altitude = 0;
        this->latitude = 0;
        this->longitude = 0;
    }
    
    SensorsData(const SensorsData &data) {
        this->altitude = data.altitude;
        this->latitude = data.latitude;
        this->longitude = data.longitude;
        this->angVel = data.angVel;
        this->accVel = data.accVel;
    }

    friend bool operator==(const SensorsData &data, const SensorsData &data2) {
        if (&data == 0 && &data2 != 0 || &data != 0 && &data2 == 0) {
            return false;
        }
        return data.altitude == data2.altitude      //
               && data.latitude == data2.latitude   //
               && data.longitude == data2.longitude //
               && data.angVel == data2.angVel       //
               && data.accVel == data2.accVel       //
            ;
    }

    friend String &operator<<(String &str, const SensorsData &ssd) {
        return str << ssd.altitude << ","
                   << ssd.latitude << ","
                   << ssd.longitude << ","
                   << ssd.angVel << ","
                   << ssd.accVel << ","
                   << ssd.errorMessage;
    }
};

} // namespace a8::link
