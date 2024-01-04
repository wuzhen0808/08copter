#pragma once
#include "a8/util/Buffer.h"
#include "a8/util/System.h"
#define A8_LED_DEBUG (0)

namespace a8::util {

class Led {
public:
    class Light {
    public:
        TimeUs timeUs;
        bool on;
        TimeUs startTimeUs = 0;
        TimeUs endTimeUs = 0;

    public:
        Light(TimeUs timeUs, bool on) {
            this->timeUs = timeUs;
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

        Mode *off(TimeUs timeUs) {
            return add(timeUs, false);
        }

        Mode *on(TimeUs timeUs) {
            return add(timeUs, true);
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

        Mode *add(TimeUs timeUs, bool on) {
            return add(new Light(timeUs, on));
        }

        Mode *add(Light *light) {
            Light *preLight = 0;
            this->lights.getLast(preLight);
            this->lights.append(light);
            light->startTimeUs = preLight == 0 ? 0 : preLight->endTimeUs;
            light->endTimeUs = light->startTimeUs + light->timeUs;
            return this;
        }
        void log(String msg) {
            sys->out->println(msg);
        }
        bool shouldOn(TimeUs startTimeUs, TimeUs timeUs) {
            TimeUs elapsedUs = timeUs - startTimeUs;
            if (A8_LED_DEBUG) {

                log(String() << "timeUs:" << timeUs << ",startTimeUs:" << startTimeUs << ",elapsedUs:" << elapsedUs);
            }

            Light *last = 0;
            lights.getLast(last);
            if (last == 0) { // no light;
                if (A8_LED_DEBUG) {
                    log(String() << "no light");
                }
                return false;
            }

            TimeUs elapsedUs2 = elapsedUs % last->endTimeUs;
            // find a light
            Light *l = 0;
            for (int i = 0; i < lights.len(); i++) {
                l = lights.get(i, 0);
                if (elapsedUs2 < l->endTimeUs) {
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
    TimeUs startTimeUs = 0;
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

    void start(TimeUs timeUs) {
        this->startTimeUs = timeUs;
    }

    void update(long off, long on) {
        update(new Mode(off, on, this->sys), true);
    }

    void update(Mode *mode, bool releaseMode) {
        this->releaseMode();
        this->mode = mode;
        this->releaseMode_ = releaseMode;
    }

    int tick(TimeUs timeUs, Result &res) {
        if (startTimeUs == 0) {
            res << "not started.";
            return -1; // do not update if not started.
        }
        if (mode == 0) {
            return 1;
        }
        bool shouldOn = mode->shouldOn(startTimeUs, timeUs);
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