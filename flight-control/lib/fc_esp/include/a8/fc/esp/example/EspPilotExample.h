#pragma once
#include "a8/fc.h"
#include "a8/fc/esp.h"
#include "a8/hal/esp/example.h"
#include "a8/util.h"
#include "a8/util/sched.h"

#include <Wire.h>
namespace a8::fc::esp::example {
using namespace a8::util;
using namespace a8::util::sched;
using namespace a8::fc;
using namespace a8::hal::esp::example;

class EspPilotExample : public BaseEspExample {
    MPU9250 *mpu;
    Buffer<Propeller *> propellers;
    LineReader *input;

public:
    EspPilotExample(MPU9250 *mpu) : BaseEspExample("EspPilotExample") {
        this->mpu = mpu;
        addPropeller(17, 50);
        addPropeller(18, 50);
        addPropeller(19, 100);
        addPropeller(20, 100);
        this->input = new LineReader(sys->getInput());
    }

    void addPropeller(int pin, int hz) {
        EspPropeller *prop = new EspPropeller();
        prop->hz(hz);
        int channel = prop->attach(pin);
        prop->setup();
        log(String() << "propeller setup,pin:" << pin << ",hz:" << hz << ",channel:" << channel);
        this->propellers.append(prop);
    }

    ~EspPilotExample() {
    }

    int setupMpu9250() {
        mpu->setup(0x68);
        mpu->selectFilter(QuatFilterSel::MADGWICK);
        mpu->setFilterIterations(1);
        log("success of setupMpu9250.");
        return 1;
    }
    int start(Result &res) override {
        this->setupSerial();
        this->setupWire();
        this->setupMpu9250();

        sch->createTask<EspPilotExample *>("EspPilotExample", this, [](EspPilotExample *this_) {
            this_->run();
        });
        return 1;
    }
    template <typename T>
    T readInputFloat(String prompt, T def) {
        Reader *reader = sys->getInput();
        T fValue = def;
        while (true) {
            sys->out->println(prompt);
            String line = readLine();
            if (line.isEmpty()) {
                // use default value.
                break;
            }
            float fValue2;
            String debug;
            int ret = Float::parseFloat<float>(line, fValue2, &debug);
            sys->out->println(debug);
            if (ret < 0) {
                sys->out->println("cannot parse string(" << line << ",len:" << line.len() << ") as float value, please re-input.");
            } else {
                fValue = (T)fValue2;
                break;
            }
        }

        return fValue;
    }
    bool confirm(String prompt) {
        sys->out->println(prompt);
        String line = readLine();
        if (line == "y" || line == "Y" || line == "") {
            return true;
        }
        return false;
    }
    String readLine() {
        String line;
        input->readLine(line, false);
        if (line.endWith('\r')) {
            line = line.subStr(0, line.len() - 1);
        }
        sys->out->println(line);
        return line;
    }

    int readConfig(Config &config) {
        while (true) {
            int i = 1;
            int total = 5;
            config.pidKp = readInputFloat(String() << i++ << "/" << total << ". Please input pid argument Kp(" << config.pidKp << "):", config.pidKp);
            config.pidKi = readInputFloat(String() << i++ << "/" << total << ". Please input pid argument Ki(" << config.pidKi << "):", config.pidKi);
            config.pidKd = readInputFloat(String() << i++ << "/" << total << ". Please input pid argument Kd(" << config.pidKd << "):", config.pidKd);
            config.flyingTimeLimitSec = readInputFloat(String() << ". Please input flyingTimeLimitSec(" << config.flyingTimeLimitSec << "):", config.flyingTimeLimitSec);
            sys->out->println(String() << "Kp:                     " << config.pidKp);
            sys->out->println(String() << "Ki:                     " << config.pidKi);
            sys->out->println(String() << "Kd:                     " << config.pidKd);
            sys->out->println(String() << "flyingTimeLimit(sec): " << config.flyingTimeLimitSec);

            if (confirm(String() << i++ << "/" << total << ". Do you confirm the config above? (y/Y/Enter)")) {
                break;
            }
        }

        return 1;
    }

    void run() {
        Config config;
        while (true) {

            int ret = readConfig(config);
            if (ret < 0) {
                continue;
            }
            Pilot *pilot = new EspPilot(config, propellers, mpu, sys, loggerFactory);
            long startTimeMs = sys->getSteadyTime();
            ret = pilot->start(startTimeMs);
            bool landing = false;
            while (true) {
                Result res;
                long timeMs = sys->getSteadyTime();
                ret = pilot->update(timeMs, res);
                if (ret < 0) {
                    log(String() << "failed to update pilot, detail:" << res);
                }
                if (pilot->isLanded()) {
                    break;
                }
                if (landing) {
                    continue;
                }
                if (timeMs - startTimeMs > config.flyingTimeLimitSec * 1000) {
                    landing = true;
                    pilot->landing(timeMs);
                }
            }

            delete pilot;
        }
    }
};
} // namespace a8::fc::esp::example