#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/config/ExitConfigItem.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class Config : public ConfigItem {

    Reader *reader;
    Output *out;
    ConfigItem *exitConfigItem;

public:
    long tickTimeMs = 10;
    long pwmElevation = 200; // 210
    long flyingTimeLimitSec = 10;
    long delayBeforeStartSec = 3;
    bool enablePropeller = false;
    long pwmMax = 2000;
    long pwmMin = 1000;
    double pidKp = 6.0;
    double pidKi = 0.0;
    double pidKd = 0.0;
    double maxBalancePidOutput = 118.0; //(-300,300)
    double maxBalancePidIntegralOutput = 75;
    bool startAfterConfig = false;

public:
    Config(Reader *reader, Output *out, PowerManage *pm, Rpy *rpy, Logger *logger) {
        this->reader = reader;
        this->out = out;
        ConfigItem *ci = this;
        this->attach(new Directory<ConfigItem *>("Root", 0));
        ConfigItems::add(ci, String() << "Power-manage", new PowerConfigItem(pm));        
        ConfigItems::add(ci, String() << "Rpy-check", new RpyConfigItem(rpy));
        ConfigItems::add(ci, "delayBeforeStartSec", delayBeforeStartSec);
        ci = ConfigItems::addReturn(ci, String() << "Pid-arguments:");
        {

            ConfigItems::add(ci, String() << "Kp", pidKp);
            ConfigItems::add(ci, String() << "Ki", pidKi);
            ConfigItems::add(ci, String() << "Kd", pidKd);
            ConfigItems::add(ci, String() << "maxBalancePidOutput", maxBalancePidOutput);
            ConfigItems::add(ci, String() << "maxBalancePidIntegralOutput", maxBalancePidIntegralOutput);
        }
        logger->debug(".....");
        ci = this;
        ConfigItems::add(ci, String() << "pwmElevation", pwmElevation);
        ConfigItems::add(ci, String() << "flyingTimeLimit(sec)", flyingTimeLimitSec);
        ConfigItems::add(ci, String() << "delayBeforeStart(sec)", delayBeforeStartSec);
        ConfigItems::add(ci, String() << "enablePropeller", enablePropeller);
        this->exitConfigItem = ConfigItems::addReturn(ci, String() << "exit", new ExitConfigItem(this->startAfterConfig));
    }

    void onLeftFailure(ConfigContext &cc) override {
        cc.navigator->to(this->exitConfigItem->getDirectory());
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