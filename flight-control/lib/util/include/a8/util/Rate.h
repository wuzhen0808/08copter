#pragma once
#include "a8/util/Math.h"

namespace a8 {
namespace util {

class Rate {
    float hz;

public:
    static Rate ForEver;

    Rate() {
        this->hz = 0.0f;
    }
    Rate(float hz) {
        this->hz = hz;
    }
    float Hz() const {
        return hz;
    }

    float mHz() const {
        return hz * 1000.0f;
    }

    float kHz() const {
        return hz * 1000;
    }

    float cps() const {
        return 1.0f / hz;
    }

    float rpm() const {
        return 60.0f / hz;
    }
    float radPs() const {
        return 2.0f * M_PI / hz;
    }

    float degPs() const {
        return 360.0f / hz;
    }

    float sec() const {
        return 1.0f / hz;
    }

    float ms() const {
        return 1000.0f / hz;
    }

    float us() const {
        return 1000000.0f / hz;
    }

    float ns() const {
        return 1000000000.0f / hz;
    }

    /**
     * Zero means no frequency.
     * Do not schedule zero rate.
     */
    bool isZero() {
        return Math::isNear(hz, 0.0f);
    }
    /*
     * ForEver means it's a runner to be called in separate thread.
     */
    bool isForEver() {
        return Math::isNear(hz, -1.0f);
    }

    Rate &
    operator=(int hz) {
        this->hz = hz;
        return *this;
    }
    Rate &operator=(const float hz) {
        this->hz = hz;
        return *this;
    }
};


} // namespace util

} // namespace a8
