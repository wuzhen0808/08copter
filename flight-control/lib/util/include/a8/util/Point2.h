#pragma once
#include "a8/util/String.h"
namespace a8::util {
template <typename T>
class Point2 {
public:
    T x;
    T y;
    Point2(T x, T y) {
        this->x = x;
        this->y = y;
    }
    friend String &operator<<(String &str, Point2 &point) {
        return str << "[" << point.x << "," << point.y << "]";
    }
};

} // namespace a8::util
