#pragma once
#include "a8/util/CodecUtil.h"

namespace a8::util {

class Vector3f {

public:
    static int write(Writer *writer, Vector3f vec) {
        int ret = 0;
        ret += CodecUtil::writeFloat32(writer, vec.x);
        ret += CodecUtil::writeFloat32(writer, vec.y);
        ret += CodecUtil::writeFloat32(writer, vec.z);
        return ret;
    }
    static int read(Reader *reader, Vector3f &vec) {
        int ret = 0;
        ret += CodecUtil::readFloat32(reader, vec.x);
        ret += CodecUtil::readFloat32(reader, vec.y);
        ret += CodecUtil::readFloat32(reader, vec.z);
        return ret;
    }

    friend bool operator==(const Vector3f &vec, const Vector3f &vec2) {
        if (&vec == 0 && &vec2 != 0 || &vec != 0 && &vec2 == 0) {
            return false;
        }
        return vec.x == vec2.x    //
               && vec.y == vec2.y //
               && vec.z == vec2.z //
            ;
    }

    friend String &operator<<(String &str, const Vector3f &vec) {
        return str << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
    }

    float x;
    float y;
    float z;
    Vector3f() { // default constructor
        x = y = z = 0;
    }
    Vector3f(Vector3f &v) { // copy constructor
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
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
