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
        Mode(long off, long on, long off2, long on2, System *sys) {
            this->sys = sys;
            this->add(off, false);
            this->add(on, true);
            this->add(off2, false);
            this->add(on2, true);
        }
        Mode(long off, long on, long off2, long on2, long off3, long on3, System *sys) {
            this->sys = sys;
            this->add(off, false);
            this->add(on, true);
            this->add(off2, false);
            this->add(on2, true);
            this->add(off3, false);
            this->add(on3, true);
        }

        ~Mode() {
            lights.clear([](Light *light) {
                delete light;
            });
        }

        Mode *off(long timeMs) {
            return add(timeMs, false);
        }

        Mode *on(long timeMs) {
            return add(timeMs, true);
        }

        Mode *offOn(long off, long on) {
            add(off, false);
            add(on, true);
            return this;
        }

        Mode *onOff(long on, long off) {
            add(on, true);
            add(off, false);
            return this;
        }

        Mode *add(long timeMs, bool on) {
            return add(new Light(timeMs, on));
        }

        Mode *add(Light *light) {
            Light *preLight = this->lights.getLast(0);
            this->lights.append(light);
            light->startTimeMs = preLight == 0 ? 0 : preLight->endTimeMs;
            light->endTimeMs = light->startTimeMs + light->timeMs;
            return this;
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
                l = lights.get(i, 0);
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

    bool releaseMode_ = true;

    void releaseMode() {
        if (this->mode == 0) {
            return;
        }
        if (this->releaseMode_) {
            delete this->mode;
        }
        this->mode = 0;
    }

public:
    Led(System *sys, int pin) {
        this->sys = sys;
        this->pin = pin;
    }
    ~Led() {
        releaseMode();
    }

    void setup() {
        this->sys->setPinMode(pin, PinMode::OUT);
    }

    void start(long timeMs) {
        this->startTimeMs = timeMs;
    }

    void update(long off, long on) {
        update(new Mode(off, on, this->sys), true);
    }

    void update(Mode *mode, bool releaseMode) {
        this->releaseMode();
        this->mode = mode;
        this->releaseMode_ = releaseMode;
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
    Mode *dangerMode;
    Mode *warnMode;
    Mode *infoMode;

public:
    StyledLed(System *sys, int pin) : Led(sys, pin) {
        this->dangerMode = new Led::Mode(sys);
        this->dangerMode->off(1000);
        for (int i = 0; i < 5; i++) {
            this->dangerMode->onOff(100, 100);
        }
        this->warnMode = new Led::Mode(900, 100, sys);
        this->infoMode = new Led::Mode(2900, 100, sys);
        this->info();
    }
    ~StyledLed() {
        delete this->dangerMode;
        delete this->warnMode;
        delete this->infoMode;
    }

    void danger() {
        this->update(this->dangerMode, false);
    }

    void warn() {
        this->update(this->warnMode, false);
    }

    void info() {
        this->update(this->infoMode, false);
    }
};
} // namespace a8::util