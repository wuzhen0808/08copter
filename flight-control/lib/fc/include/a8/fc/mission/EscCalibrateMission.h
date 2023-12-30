#pragma once
#include "a8/fc/mission/Mission.h"
#include "a8/fc/pwm/EmptyPwmCalculator.h"
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util.h"

namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

class EscCalibrateMission : public Mission {
    class PropellerStatus {
    public:
        long pwm = 1000;
        void add(long step) {
            long pwm = this->pwm + step;
            if (pwm > 2000) {
                pwm = 2000;
            }
            if (pwm < 1000) {
                pwm = 1000;
            }
            this->pwm = pwm;
        }
        void max() {
            this->pwm = 2000;
        }
        void min() {
            this->pwm = 1000;
        }
    };
    class MissionConfigItem : public ConfigItem {

        void onAttached() {
        }
    };

    class Foreground : public ConfigItem {
        EscCalibrateMission *mission;

    public:
        Foreground(EscCalibrateMission *mission) {
            this->mission = mission;
        }

        void addAction(String name, void (*onBuildTitle)(TitleBuilder &, EscCalibrateMission *), void (*action)(ConfigContext &, EscCalibrateMission *)) {
            struct Context {
                void (*action)(ConfigContext &, EscCalibrateMission *);
                EscCalibrateMission *mission;
                void (*onBuildTitle)(TitleBuilder &, EscCalibrateMission *);

            } *c = new Context();
            c->action = action;
            c->mission = this->mission;
            c->onBuildTitle = onBuildTitle;

            ConfigItem *ci = ConfigItems::addReturn(this, name);
            ci->setAttribute(0, c, Lang::release<Context>);

            ci->onEnter = [](ConfigContext &cc) {
                Context *c = cc.navigator->get()->getAttribute<Context *>(0);
                c->action(cc, c->mission);
            };
            ci->onBuildTitle = [](TitleBuilder &title) {
                Context *c = title.configItem->getAttribute<Context *>(0);
                c->onBuildTitle(title, c->mission);
            };
        }

        void onAttached() override {

            // actions
            void (*onBuildTitle)(TitleBuilder &, EscCalibrateMission *) = [](TitleBuilder &title, EscCalibrateMission *mission) {
                mission->buildTitle(title);
            };
            Buffer<String> options;
            options.add("propeller0");
            options.add("propeller1");
            options.add("propeller2");
            options.add("propeller3");
            options.add("All");

            ConfigItems::addSelectInput(this, "Select-Propeller", this->mission->propeller, options);
            ConfigItems::add(this, "Step", this->mission->step);
            this->addAction(
                "+", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->plus();
                });

            this->addAction(
                "-", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->minus();
                });
            this->addAction(
                "Max(2000)", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->max();
                });
            this->addAction(
                "Min(1000)", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->min();
                });

            {
                ConfigItem *ci = ConfigItems::addReturn(this, "recentMessage");
                ci->onBuildTitle = [](TitleBuilder &title) {
                    Foreground *fg = title.configItem->getRoot<Foreground>();
                    title.set<String>("message", fg->mission->message);
                };
            }

            this->addAction(
                "Exit", [](TitleBuilder &title, EscCalibrateMission *mission) { //
                    title.set<bool>("Running", mission->running);               //
                },                                                              //
                [](ConfigContext &cc, EscCalibrateMission *mission) {           //
                    cc.navigator->stop(1);                                      //
                    mission->exit();                                            //
                });
        }
        void enter(ConfigContext &cc) override {
            ConfigItems::runNav(this->dir, cc);
        }
    }; // end of Foreground.

protected:
    Propellers *propellers;
    Foreground *fg;
    PwmCalculator *pwmCalculator;

    Buffer<PropellerStatus *> pss;

    long step = 5;

    int propeller;

    bool running = true;

    String message;

public:
    EscCalibrateMission(long id, PowerManage * pm, Propellers *propellers, Collector *collector, ConfigContext &cc, Throttle &throttle, SyncQueue<int> *signalQueue, System *sys, LoggerFactory *logFac)
        : Mission(id, cc, throttle, signalQueue, collector, sys, logFac, "EscCalibrateMission") {
        this->propellers = propellers;
        this->fg = new Foreground(this);

        this->propeller = -1;
        this->pss.add(new PropellerStatus());
        this->pss.add(new PropellerStatus());
        this->pss.add(new PropellerStatus());
        this->pss.add(new PropellerStatus());

        this->pwmCalculator = new EmptyPwmCalculator(pm, logFac);
    }

    ~EscCalibrateMission() {
        pss.clear(Lang::delete_<PropellerStatus>);
        delete this->pwmCalculator;
        delete this->fg;
    }

    void exit() {
        this->running = false;
    }
    int setup(Result &res) override {
        propellers->setPwmCalculator(this->pwmCalculator);
        return 1;
    }

    ConfigItem *getForeground() override {
        return this->fg;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) {

        for (int i = 0; i < pss.len(); i++) {
            PropellerStatus *ps = pss.get(i, 0);
            title.set<long>(String() << "prop" << i, ps->pwm);
        }
    }

    template <typename C>
    void forEachPs(C c, void (*consumer)(C, PropellerStatus *)) {
        if (this->propeller < 0 || this->propeller >= pss.len()) { // ALL.
            for (int i = 0; i < pss.len(); i++) {
                PropellerStatus *ps = pss.get(i, 0);
                consumer(c, ps);
            }
        } else {
            PropellerStatus *ps = pss.get(this->propeller, 0);
            consumer(c, ps);
        }
    }
    void plus() {
        forEachPs<EscCalibrateMission *>(this, [](EscCalibrateMission *mission, PropellerStatus *ps) { ps->add(mission->step); });
    }
    void minus() {
        forEachPs<EscCalibrateMission *>(this, [](EscCalibrateMission *mission, PropellerStatus *ps) { ps->add(-mission->step); });
    }
    void max() {
        forEachPs<EscCalibrateMission *>(this, [](EscCalibrateMission *mission, PropellerStatus *ps) { ps->max(); });
    }
    void min() {
        forEachPs<EscCalibrateMission *>(this, [](EscCalibrateMission *mission, PropellerStatus *ps) { ps->min(); });
    }
    int run(Result &res) override {

        int ret = propellers->isReady(res);
        if (ret < 0) {
            this->running = false;
            return ret;
        }

        while (this->running) {
            this->message.clear();
            for (int i = 0; i < pss.len(); i++) {
                PropellerStatus *ps = pss.get(i, 0);
                Propeller *prop = propellers->get(i);
                bool enabled = prop->isEnabled();
                if (enabled) {
                    prop->setThrottle(ps->pwm);
                    prop->commitUpdate(propellers->getPwmCalculator());
                }
                this->message << "prop" << i << ":" << (enabled ? "enabled " : "disabled ") //
                              << ",pwm:" << ps->pwm << "=>" << prop->getPwm() << ";";
            }
            this->sys->delay(6); // delay 2ms,2000us
        }

        return 1;
    }
};

} // namespace a8::fc::mission