#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/System.h"
#define A8_LED_DEBUG (0)

namespace a8::util {

class Led {
public:
    class Light {
    public:
        long timeMs;
        bool on;
        long startTimeMs = -1;
        long endTimeMs = -1;

    public:
        Light(long timeMs, bool on) {
            this->timeMs = timeMs;
            this->on = on;
        }
    };

    class Mode {
        Buffer<Light *> lights;
        System *sys;

    public:
        Mode(System *sys) {
            this->sys = sys;
        }
        Mode(long off, long on, System *sys) {
            this->sys = sys;
            this->add(off, false);
            this->add(on, true);
        }
        ~Mode() {
            lights.clear([](Light *light) {
                delete light;
            });
        }
        void add(long timeMs, bool on) {
            this->add(new Light(timeMs, on));
        }

        void add(Light *light) {
            Light *preLight = this->lights.getLast(0);
            this->lights.append(light);
            light->startTimeMs = preLight == 0 ? 0 : preLight->endTimeMs;
            light->endTimeMs = light->startTimeMs + light->timeMs;
        }
        void log(String msg) {
            sys->out->println(msg);
        }
        bool shouldOn(long startTimeMs, long timeMs) {
            long elapsed = timeMs - startTimeMs;
            if (A8_LED_DEBUG) {

                log(String() << "timeMs:" << timeMs << ",startTimeMs:" << startTimeMs << ",elapsed:" << elapsed);
            }

            Light *last = lights.getLast(0);
            if (last == 0) { // no light;
                if (A8_LED_DEBUG) {
                    log(String() << "no light");
                }
                return false;
            }

            long elapsed2 = elapsed % last->endTimeMs;
            // find a light
            Light *l = 0;
            for (int i = 0; i < lights.len(); i++) {
                l = lights.get(i);
                if (elapsed2 < l->endTimeMs) {
                    if (A8_LED_DEBUG) {
                        log(String() << "found light:" << i);
                    }
                    break; // found a light.
                }
            }
            return l->on;
        }
    };

protected:
    int pin;
    long startTimeMs = -1;
    bool on_ = false; // status of the led.
    System *sys;
    Mode *mode = 0;

public:
    Led(System *sys, int pin) {
        this->sys = sys;
        this->pin = pin;
    }
    ~Led() {
        releaseMode();
    }
    void releaseMode() {
        if (this->mode == 0) {
            return;
        }
        delete this->mode;
        this->mode = 0;
    }

    void setup() {
        this->sys->setPinMode(pin, PinMode::OUT);
    }

    void start(long timeMs) {
        this->startTimeMs = timeMs;
    }

    void update(long off, long on) {
        update(new Mode(off, on, this->sys));
    }
    void update(Mode *mode) {
        releaseMode();
        this->mode = mode;
    }

    int tick(long timeMs, Result &res) {
        if (startTimeMs < 0) {
            res << "not started.";
            return -1; // do not update if not started.
        }
        if (mode == 0) {
            return 1;
        }
        bool shouldOn = mode->shouldOn(startTimeMs, timeMs);
        if (shouldOn != this->on_) {
            sys->digitalWrite(this->pin, shouldOn);
            this->on_ = shouldOn;
            if (this->on_) {
                return 2;
            } else {
                return 3;
            }
        }

        return 5;
    }
}; // end of Led.

class StyledLed : public Led {
    enum Style {
        DANGER,
        WARN,
        INFO,
        UNKNOWN
    };
    Style style = UNKNOWN;

public:
    StyledLed(System *sys, int pin) : Led(sys, pin) {
        this->info();
    }
    void danger() {
        if (style == DANGER) {
            return;
        }
        update(new Led::Mode(200, 200, this->sys));
        style = DANGER;
    }

    void warn() {
        if (style == WARN) {
            return;
        }
        update(new Mode(1900, 200, this->sys));
        style = WARN;
    }

    void info() {
        if (style == INFO) {
            return;
        }
        update(new Mode(2900, 200, this->sys));
        style = INFO;
    }
};
} // namespace a8::util