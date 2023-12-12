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
    EspPropellers *propellers;
    LineReader *input;

public:
    EspPilotExample(MPU9250 *mpu) : BaseEspExample("EspPilotExample") {
        this->mpu = mpu;
        this->propellers = new EspPropellers(17, 18, 19, 20, loggerFactory);
        this->input = new LineReader(sys->getInput());
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

    bool confirm(String prompt, bool def) {
        return select(prompt, "Yes", "No", def ? 0 : 1);
    }

    bool readBool(String prompt, bool def) {
        return 0 == select(prompt, "Yes", "No", def ? 0 : 1);
    }

    int select(String prompt, String option0, String option1) {
        return select(prompt, option0, option1, 0);
    }
    int select(String prompt, String option0, String option1, int def) {
        int ret = def;
        while (true) {

            sys->out->println(prompt);
            sys->out->println(String() << " " << 0 << (def == 0 ? "*" : " ") << ":" << option0);
            sys->out->println(String() << " " << 1 << (def == 1 ? "*" : " ") << ":" << option1);

            String selected = readLine();
            if (selected == "") {
                break; // use default
            }
            if (selected == "0") {
                ret = 0;
                break;
            }
            if (selected == "1") {
                ret = 1;
                break;
            }
            sys->out->println(String() << "no  such option:" << selected);
        }
        return ret;
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
    void step(int &step, int total) {
        step++;
        sys->out->println(String() << "Step " << step << " of " << total);
    }

    int readConfig(Config &config) {
        while (true) {
            int i = 0;
            int total = 10;
            step(i, total);
            config.maxBalancePidOutput = readInputFloat(String() << " Please input maxBalancePidOutput(" << config.maxBalancePidOutput << "):", config.maxBalancePidOutput);
            step(i, total);
            config.maxBalancePidIntegralOutput = readInputFloat(String() << " Please input maxPidIntegralOutput(" << config.maxBalancePidIntegralOutput << "):", config.maxBalancePidIntegralOutput);
            step(i, total);
            config.pidKp = readInputFloat(String() << total << " Please input pid argument Kp(" << config.pidKp << "):", config.pidKp);
            step(i, total);
            config.pidKi = readInputFloat(String() << total << " Please input pid argument Ki(" << config.pidKi << "):", config.pidKi);
            step(i, total);
            config.pidKd = readInputFloat(String() << total << " Please input pid argument Kd(" << config.pidKd << "):", config.pidKd);
            step(i, total);
            config.pwmElevation = readInputFloat(String() << " Please input pwmElevation argument (" << config.pwmElevation << "):", config.pwmElevation);
            step(i, total);
            config.flyingTimeLimitSec = readInputFloat(String() << " Please input flyingTimeLimitSec(" << config.flyingTimeLimitSec << "):", config.flyingTimeLimitSec);
            step(i, total);
            config.delayBeforeStartSec = readInputFloat(String() << " Please input delayBeforeStartSec(" << config.delayBeforeStartSec << "):", config.delayBeforeStartSec);
            step(i, total);
            config.enablePropeller = readBool(String() << "Enable propeller?", config.enablePropeller);
            step(i, total);
            //
            sys->out->println(String() << "Configuration: ");

            sys->out->println(String() << " Pid arguments:");
            sys->out->println(String() << "  maxBalancePidOutput: " << config.maxBalancePidOutput );
            sys->out->println(String() << "  maxBalancePidIntegralOutput: " << config.maxBalancePidIntegralOutput );
            sys->out->println(String() << "  Kp:                     " << config.pidKp);
            sys->out->println(String() << "  Ki:                     " << config.pidKi);
            sys->out->println(String() << "  Kd:                     " << config.pidKd);
            sys->out->println(String() << " pwmElevation:           " << config.pwmElevation);
            sys->out->println(String() << " flyingTimeLimit(sec): " << config.flyingTimeLimitSec);
            sys->out->println(String() << " delayBeforeStart(sec): " << config.delayBeforeStartSec);
            sys->out->println(String() << " enablePropeller: " << (config.enablePropeller ? "Y" : "N"));

            if (0 == select("Where to go?",                                                                               //
                            String() << "Use above config and start after " << config.delayBeforeStartSec << " seconds.", //
                            "Re-configure.")) {
                break;
            }
            // reconfigure
        }

        return 1;
    }
    int doRun(Config &config, Pilot *pilot, Result res) {
        long startTimeMs = sys->getSteadyTime();
        propellers->enableAll(config.enablePropeller);
        throttle::Context context(startTimeMs, this->propellers);
        int ret = pilot->start(context, res);
        if (ret < 0) {
            return ret;
        }

        // wait 3 secs.
        sys->out->println(String() << "delay ... " << config.delayBeforeStartSec << " sec before start.");
        delay(config.delayBeforeStartSec * 1000);

        bool landing = false;
        long preTimeMs = sys->getSteadyTime();
        while (true) {
            Result res;
            long timeMs = sys->getSteadyTime();
            long remainMs = config.tickTimeMs - (timeMs - preTimeMs);
            if (remainMs > 0) {
                delay(remainMs);
            }
            ret = pilot->update(&context, timeMs, res);

            if (ret < 0) {
                log(String() << "failed to update pilot, detail:" << res);
            }

            if (pilot->isLanded()) {
                break;
            }

            if (!landing) { //
                // check if need start landing.
                if (timeMs - startTimeMs > config.flyingTimeLimitSec * 1000) {
                    landing = true;
                    pilot->landing(timeMs);
                }
            }
            preTimeMs = timeMs;
        }
        return 1;
    }

    void run() {
        Config config;
        Rpy *rpy = new EspRpy(mpu, loggerFactory);

        while (true) {

            // get rpy ready.
            while (true) {
                Result res;
                int ret = rpy->checkIfReady(res);
                if (ret < 0) {
                    log(res.errorMessage);

                    if (0 == select("Where to go?",
                                    String() << "Force start with the current rpy number(Roll:" << rpy->getRoll() << ",Pitch:" << rpy->getPitch() << ")", //
                                    "Continue to wait the rpy ready.", 0)) {
                        break;
                    }
                }
                log("rpy is ready.");
                break;
            }

            int ret = readConfig(config);
            if (ret < 0) {
                continue;
            }

            Pilot *pilot = new EspPilot(config, rpy, propellers, mpu, sys, loggerFactory);
            Result res;
            ret = this->doRun(config, pilot, res);
            String msg;
            propellers->printHistory(msg);
            pilot->printHistory(0, msg);
            this->sys->out->println(msg);
            delete pilot;
            if (ret < 0) {
                log(res.errorMessage);
            }
        }
    }
};
} // namespace a8::fc::esp::example