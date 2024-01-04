#pragma once
#include "a8/util/Math.h"
#include "a8/util/String.h"
namespace a8 {
namespace util {

class Rate {
    float hz;

public:
    static Rate RUN;

    static Rate ms(long ms) {
        return A8_MS_PER_SEC / (double)ms;
    }
    
    static Rate us(unsigned long us) {
        return A8_US_PER_SEC / (double)us;
    }

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
        return hz * 1.0e3f;
    }

    float kHz() const {
        return hz * 1.0e-3f;
    }
    /**
     * circle per second
     */
    float cps() const {
        return hz;
    }
    /**
     * rotation per minute
     */
    float rpm() const {
        return 60.0f * hz;
    }
    float radPs() const {
        return 2.0f * M_PI_ * hz;
    }

    float degPs() const {
        return 360.0f * hz;
    }
    /**
     * seconds per rotation
     */
    float sec() const {
        if (isZero()) {
            return -1;
        }
        return 1.0f / hz;
    }
    float us() const {
        if (isZero()) {
            return -1;
        }
        return 1.0e6 * sec();
    }
    /**
     * millisec per round
     */
    float ms() const {
        if (isZero()) {
            return -1;
        }
        return 1.0e3 * sec();
    }

    /**
     * Zero means no frequency.
     * Do not schedule zero rate.
     */
    bool isZero() const {
        return (int)mHz() == 0;
    }
    /*
     * RUN means it's a runner to be called in separate thread.
     */
    bool isRun() const {
        return (int)hz == -1;
    }

    Rate &operator=(int hz) {
        this->hz = hz;
        return *this;
    }
    Rate &operator=(const float hz) {
        this->hz = hz;
        return *this;
    }

    //
    friend String &operator<<(String &str, const Rate &rate) {
        return str << rate.hz;
    }
};

} // namespace util

} // namespace a8
