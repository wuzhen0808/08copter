#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/fc/config/StartConfigItem.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
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

public:
    long tickTimeMs = 1;
    long pwmElevation = 210; // 210
    long flyingTimeLimitSec = 10;
    long delayBeforeStartSec = 3;
    bool enablePropeller = true;
    long pwmMax = 2000;
    long pwmMin = 1000;
    double pidKp = 5.2; //
    double pidKi = 1.0;
    double pidKd = 1.65;
    double pidOutputLimit = 340.0; //
    double pidOutputLimitI = 100;
    bool startAfterConfig = false;

public:
    Config(Reader *reader, Output *out, PowerManage *pm, Rpy *rpy, Logger *logger) {
        this->reader = reader;
        this->out = out;
        ConfigItem *ci = this;
        this->attach(new Directory<ConfigItem *>("Root", 0));
        this->startConfigItem = ConfigItems::addReturn(ci, String() << "Start-now", new StartConfigItem(startAfterConfig));
        ConfigItems::add(ci, "Power-manage", new PowerConfigItem(pm));
        ConfigItems::add(ci, "tickTimeMs", tickTimeMs);
        ConfigItems::add(ci, "flyingTimeLimit(sec)", flyingTimeLimitSec);
        ConfigItems::add(ci, "enablePropeller", enablePropeller);
        ConfigItems::add(ci, "startAfterConfig", this->startAfterConfig);
        ConfigItems::add(ci, "delayBeforeStartSec", delayBeforeStartSec);
        ConfigItems::add(ci, "pwmElevation", pwmElevation);
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

    void onLeftFailure(ConfigContext &cc) override {
        cc.navigator->to(this->startConfigItem->getDirectory());
    }

    void enter(ConfigContext &cc) override {

        cc.logger->debug(">>Config::config.");
        DirectoryNavigator<ConfigContext &, ConfigItem *> nav(cc.reader, cc.out, this->tree);
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