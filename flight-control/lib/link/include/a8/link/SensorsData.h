#pragma once

#include "a8/util.h"
#include "a8/util/net.h"

using namespace a8::util;

namespace a8::link {

// Stub to ground station interface.
struct SensorsData {

    static int write_(Writer *writer, SensorsData* data) {
        int ret = 0;
        ret += CodecUtil::writeDouble(writer, data->altitude);
        ret += CodecUtil::writeDouble(writer, data->latitude);
        ret += CodecUtil::writeDouble(writer, data->longitude);
        ret += Vector3f::write(writer, data->angVel);
        ret += Vector3f::write(writer, data->accVel);
        return ret;
    }
    static int read_(Reader *reader, SensorsData *&data) {
        int ret = 0;
        ret += CodecUtil::readDouble(reader, data->altitude);
        ret += CodecUtil::readDouble(reader, data->latitude);
        ret += CodecUtil::readDouble(reader, data->longitude);
        ret += Vector3f::read(reader, data->angVel);
        ret += Vector3f::read(reader, data->accVel);
        ret += CodecUtil::readString(reader, data->errorMessage);
        return ret;
    }

    static void free(SensorsData * data){
        Lang::free(data);
    }

    double altitude;
    double latitude;
    double longitude;
    Vector3f angVel; // angular velocity
    Vector3f accVel; // acceleration velocity
    String errorMessage;
    
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
