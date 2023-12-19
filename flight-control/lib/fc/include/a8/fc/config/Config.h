#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/RpyMonitor.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/fc/config/StartConfigItem.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
enum UnBalanceAction {
    ASK = 0,
    IGNORE = 1,
    IGNORE_IF_SAFE = 2,
    END_OF_MISSION = 3
};

/**
 * Test report:
 * pwmElevation,P,      I,  D,  pidLimit(actual),  pidLimitI(actual), actualMaxPwm
 * 200,         6.0,    0,  0,  150(-158),          75(0),
 *
 */
class Config : public ConfigItem {

    Reader *reader;
    Output *out;
    ConfigItem *startConfigItem;
    Scheduler *sch;
    RpyMonitor *rpyMonitor;

public:
    long tickTimeMs = 2;
    float elevationThrottle = 210.0f; // 210
    long flyingTimeLimitSec = 10;
    long delayBeforeStartSec = 3;
    bool enablePropeller = false;
    int stableCheckRetries = 5;
    int unBalanceAction = UnBalanceAction::IGNORE_IF_SAFE;
    float maxThrottle = 1000;
    double pidKp = 5.2; //
    double pidKi = 1.0;
    double pidKd = 1.65;
    double pidOutputLimit = 340.0; //
    double pidOutputLimitI = 100;
    bool enableStart = true;

public:
    Config(Reader *reader, Output *out, PowerManage *pm, Rpy *rpy, Logger *logger, History &his, Scheduler *sch) {
        this->reader = reader;
        this->out = out;
        this->sch = sch;
        this->rpyMonitor = new RpyMonitor(rpy, out, sch);
        ConfigItem *ci = this;
        this->attach(new Directory<ConfigItem *>("Root", 0));
        this->startConfigItem = ConfigItems::addReturn(ci, String() << "Start", new StartConfigItem(enableStart));
        ci = this->startConfigItem;
        {
            ConfigItems::addSelectInput<Config *>(
                ci, "unBalanceAction", unBalanceAction, this, 4, [](Config *this_, int idx) {
                    String option;
                    switch (idx) {
                    case 0:
                        option = "ASK";
                        break;
                    case 1:
                        option = "IGNORE";
                        break;
                    case 2:
                        option = "IGNORE_IF_SAFE";
                        break;
                    case 3:
                        option = "END_OF_MISSION";
                        break;
                    }
                    return option;
                });
            ConfigItems::add(ci, "stableCheckRetries", stableCheckRetries);
            ConfigItems::add(ci, "enablePropeller", enablePropeller);
            ConfigItems::add(ci, "enableStart", this->enableStart);
        }

        ci = this;
        ci = ConfigItems::addReturn(ci, "Print-history");
        {
            ci->setAttribute(&his, [](void *) {});
            ci->onEnter = [](ConfigContext &cc) {
                History *his = cc.navigator->get()->getAttribute<History *>(0);
                his->print(cc.out);
            };
        }

        ci = this;
        ci = ConfigItems::addReturn(ci, "Monitor-rpy");
        {
            ci->setAttribute(rpyMonitor, [](void *) {});
            ci->onBuildTitle = [](TitleBuilder &title) {
                RpyMonitor *rpyMonitor = title.configItem->getAttribute<RpyMonitor *>(0);
                title.set<bool>("Running", rpyMonitor->isRunning());
            };
            ci->onEnter = [](ConfigContext &cc) {
                RpyMonitor *rpyMonitor = cc.navigator->get()->getAttribute<RpyMonitor *>(0);
                if (rpyMonitor->isRunning()) {
                    rpyMonitor->close();
                } else {
                    rpyMonitor->open();
                }
            };
            ConfigItems::add<RpyMonitor *, float>(
                ci, "Set-rate", rpyMonitor,                                           //
                [](RpyMonitor *rpyMonitor) { return rpyMonitor->getRate().Hz(); },    //
                [](RpyMonitor *rpyMonitor, float rate) { rpyMonitor->setRate(rate); } //
            );
            {
            }
        }

        ci = this;
        ConfigItems::add(ci, "Power-manage", new PowerConfigItem(pm));
        ConfigItems::add(ci, "tickTimeMs", tickTimeMs);
        ConfigItems::add(ci, "flyingTimeLimit(sec)", flyingTimeLimitSec);
        ConfigItems::add(ci, "delayBeforeStartSec", delayBeforeStartSec);
        ConfigItems::add(ci, "maxThrottle", maxThrottle);
        ConfigItems::add(ci, "Rpy-check", new RpyConfigItem(rpy));
        ci = ConfigItems::addReturn(ci, "Pid-arguments:");
        {

            ConfigItems::add(ci, "pidOutputLimit", pidOutputLimit);
            ConfigItems::add(ci, "pidOutputLimitI", pidOutputLimitI);
            ConfigItems::add(ci, "Kp", pidKp);
            ConfigItems::add(ci, "Ki", pidKi);
            ConfigItems::add(ci, "Kd", pidKd);
        }
    }
    ~Config() {
        delete this->rpyMonitor;
    }

    void onLeftFailure(ConfigContext &cc) override {
        cc.navigator->to(this->startConfigItem->getDirectory());
    }

    void enter(ConfigContext &cc) override {

        cc.logger->debug(">>Config::config.");
        DirectoryNavigator<ConfigContext &, ConfigItem *> nav(cc.reader, cc.out, this->dir);
        cc.navigator = &nav;
        nav.setEnterHandler([](ConfigContext &cc, DirectoryNavigator<ConfigContext &, ConfigItem *> *nav) {
            cc.logger->debug("dir enter.");
            Directory<ConfigItem *> *dir = nav->get();
            if (dir->isRoot()) {
                // ignore root,avoid recursive config.
                return;
            }
            ConfigItem *ci = dir->getElement();
            ci->enter(cc);
        });
        nav.setLeftHandler([](ConfigContext &cc, DirectoryNavigator<ConfigContext &, ConfigItem *> *nav) {
            bool changed = nav->left();
            if (!changed) {
                // go to start config item if at root node & press left key.
                Directory<ConfigItem *> *dir = nav->get();
                ConfigItem *ci = dir->getElement();
                ci->onLeftFailure(cc);
            }
        });

        cc.logger->debug(String() << "run...");

        int ret = nav.run(cc); // blocked here until stop nav.

        cc.logger->debug(String() << "<<Config::config,ret:" << ret);
    }
};
} // namespace a8::fc