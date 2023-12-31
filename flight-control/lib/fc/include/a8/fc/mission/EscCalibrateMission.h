#pragma once
#include "a8/fc/mission/Mission.h"
#include "a8/fc/pwm/EmptyPwmCalculator.h"
#include "a8/fc/pwm/PwmCalculator.h"
#include "a8/util.h"

namespace a8::fc::mission {
using namespace a8::util;
using namespace a8::fc::throttle;

const int S_OPEN = 1;
const int S_CLOSE = 2;
const int S_LOCK = 3;
const int S_UNLOCK = 4;
const int S_PWM_SET = 5;
const int S_PWM_MIN = 6;
const int S_PWM_MAX = 7;
const int S_PWM_INCREASE = 8;
const int S_PWM_DECREASE = 9;
const int S_COMMIT = 10;

class EscCalibrateMission : public Mission {

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
            ConfigItem *ci = this;
            ci = ConfigItems::addReturn(ci, "Select-Propeller");
            {
                ci->setAttribute(0, mission, Lang::empty<void *>);
                ci->onBuildTitle = [](TitleBuilder &title) {
                    EscCalibrateMission *mission = title.configItem->getAttribute<EscCalibrateMission *>(0, 0);
                    String selected;
                    String unselected;
                    for (int i = 0; i < 4; i++) {
                        if (mission->isSelected(i)) {
                            selected << i << ",";
                        } else {
                            unselected << i << ",";
                        }
                    }
                    title.set<String>("selected", selected);
                    title.set<String>("unselected", unselected);
                };
                for (int i = 0; i < 4; i++) {
                    ConfigItem *cii = ConfigItems::addReturn(ci, String() << "Prop" << i);
                    cii->setAttribute(0, mission, Lang::empty<void *>);
                    cii->setAttribute(1, mission->propellers->get(i), Lang::empty<void *>);
                    cii->onBuildTitle = [](TitleBuilder &title) {
                        EscCalibrateMission *mission = title.configItem->getAttribute<EscCalibrateMission *>(0, 0);
                        Propeller *propI = title.configItem->getAttribute<Propeller *>(1, 0);
                        title.set<bool>("lock", propI->isLocked());
                        title.set<bool>("open", propI->isOpen());
                        title.set<long>("pwm", propI->getPwm());
                        title.set<bool>("selected:", mission->isSelected(propI->getIndex()));
                    };
                    cii->onEnter = [](ConfigContext &cc) {
                        EscCalibrateMission *mission = cc.getConfigItem()->getAttribute<EscCalibrateMission *>(0, 0);
                        Propeller *propI = cc.getConfigItem()->getAttribute<Propeller *>(1, 0);
                        mission->changeSelect(propI->getIndex());
                    };
                }

                ci = this;
            }

            ConfigItems::add(ci, "Step", this->mission->step);
            this->addAction(
                "+", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_PWM_INCREASE);
                    mission->signal(S_COMMIT);
                });

            this->addAction(
                "-", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_PWM_DECREASE);
                    mission->signal(S_COMMIT);
                });
            this->addAction(
                String() << "Max(" << GlobalVars::MAX_PWM << ")", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_PWM_MAX);
                    mission->signal(S_COMMIT);
                });
            this->addAction(
                String() << "Min(" << GlobalVars::MIN_PWM << ")", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_PWM_MIN);
                    mission->signal(S_COMMIT);
                });
            this->addAction(
                "Lock", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_LOCK);
                });
            this->addAction(
                "UnLock", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_UNLOCK);
                });
            this->addAction(
                "Open", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_OPEN);
                });
            this->addAction(
                "Close", onBuildTitle, [](ConfigContext &, EscCalibrateMission *mission) {
                    mission->signal(S_CLOSE);
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

    long step = 5;

    bool propellerSelect[4] = {true, true, true, true};

    bool running = true;

    String message;

public:
    EscCalibrateMission(long id, PowerManage *pm, Propellers *propellers, Collector *collector, ConfigContext &cc, Throttle &throttle, SyncQueue<int> *signalQueue, System *sys, LoggerFactory *logFac)
        : Mission(id, cc, throttle, signalQueue, collector, sys, logFac, "EscCalibrateMission") {
        this->propellers = propellers;
        this->fg = new Foreground(this);

        this->pwmCalculator = new EmptyPwmCalculator(pm, logFac);
    }

    ~EscCalibrateMission() {
        delete this->pwmCalculator;
        delete this->fg;
    }
    bool isSelected(int idx) {
        return propellerSelect[idx];
    }
    void changeSelect(int idx) {
        bool select = isSelected(idx);
        propellerSelect[idx] = !select;
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
        Buffer<Propeller *> ps = propellers->getAll();
        for (int i = 0; i < ps.len(); i++) {
            title.set<long>(String() << "prop" << i, BufferUtil::get(ps, i)->getPwm());
        }
    }

    template <typename C>
    void forEachPropellerSelected(C c, void (*consumer)(C, Propeller *)) {
        Buffer<Propeller *> ps = propellers->getAll();
        for (int i = 0; i < ps.len(); i++) {
            if (this->isSelected(i)) {
                consumer(c, BufferUtil::get(ps, i));
            }
        }
    }

    int run(Result &res) override {

        int ret = propellers->isReady(res);
        if (ret < 0) {
            this->running = false;
            return ret;
        }

        int signal;
        while (this->running) {
            this->message.clear();
            message << "last signal:" << signal;
            this->forEachPropellerSelected<int>(0, [](int, Propeller *prop) {
                prop->commit();
            });
            int got = this->signalQueue->take(signal, 6);
            if (!got) {
                continue;
            }
            switch (signal) {
            case S_CLOSE:
                this->forEachPropellerSelected<int>(0, [](int, Propeller *prop) {
                    prop->close();
                });
                break;
            case S_OPEN:
                this->forEachPropellerSelected<int>(0, [](int, Propeller *prop) {
                    prop->open();
                });
                break;
            case S_LOCK:
                this->forEachPropellerSelected<int>(0, [](int, Propeller *prop) {
                    prop->lock();
                });
                break;
            case S_UNLOCK:
                this->forEachPropellerSelected<int>(0, [](int, Propeller *prop) {
                    prop->unLock();
                });
                break;
            case S_PWM_INCREASE:
                this->forEachPropellerSelected<int>(this->step, [](int step, Propeller *prop) {
                    long pwm = prop->getPwm();
                    prop->setPwm(pwm + step);
                });
                break;
            case S_PWM_DECREASE:
                this->forEachPropellerSelected<int>(this->step, [](int step, Propeller *prop) {
                    long pwm = prop->getPwm();
                    prop->setPwm(pwm - step);
                });
                break;
            case S_PWM_MAX:
                this->forEachPropellerSelected<int>(this->step, [](int step, Propeller *prop) {
                    prop->setPwm(GlobalVars::MAX_PWM);
                });
                break;
            case S_PWM_MIN:
                this->forEachPropellerSelected<int>(this->step, [](int step, Propeller *prop) {
                    prop->setPwm(GlobalVars::MIN_PWM);
                });
                break;
            }
        }

        return 1;
    }
};

} // namespace a8::fc::mission