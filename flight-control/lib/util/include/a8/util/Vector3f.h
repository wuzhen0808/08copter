#pragma once
#include "a8/util/CodecUtil.h"

namespace a8::util {

class Vector3f {

public:
    static int write(Writer *writer, Vector3f vec) {
        int ret = 0;
        ret += CodecUtil::writeFloat(writer, vec.x);
        ret += CodecUtil::writeFloat(writer, vec.y);
        ret += CodecUtil::writeFloat(writer, vec.z);
        return ret;
    }
    static int read(Reader *reader, Vector3f &vec) {
        int ret = 0;
        ret += CodecUtil::readFloat(reader, vec.x);
        ret += CodecUtil::readFloat(reader, vec.y);
        ret += CodecUtil::readFloat(reader, vec.z);
        return ret;
    }

    friend String &operator<<(String &str, const Vector3f &vec) {
        return str << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
    }

    float x;
    float y;
    float z;
    Vector3f() {
        x = y = z = 0;
    }
    Vector3f(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3f &operator=(float v) {
        x = y = z = v;
        return *this;
    }
    Vector3f &operator*(float f) {
        this->x *= f;
        this->y *= f;
        this->z *= f;
        return *this;
    }
};

} // namespace a8::util
