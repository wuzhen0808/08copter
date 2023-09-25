#pragma once
#include "a8/util/Math.h"

namespace a8 {
namespace util {

class Rate {
    float hz;

public:
    Rate() {
        this->hz = 0.0f;
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

    bool isZero() {
        return Math::isNearZero(hz);
    }

    Rate &
    operator=(int hz) {
        this->hz = hz;
        return *this;
    }
    Rate &operator=(float hz) {
        this->hz = hz;
        return *this;
    }
};
} // namespace util

} // namespace a8
