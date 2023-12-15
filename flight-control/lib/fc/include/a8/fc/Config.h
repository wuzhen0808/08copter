#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class RpyStableCheckConfigItem : public ConfigItem {
    Rpy *rpy;
    bool forceStart = false;
    bool stable = false;

public:
    RpyStableCheckConfigItem(Rpy *rpy) {
        this->rpy = rpy;
    }

    String getTitle(String name) override {
        return String() << name << "(stable:" << stable << "forceStart:" << forceStart << ")";
    }

    void onAttached() override {
        {

            ConfigItem *ci = ConfigItems::addReturn(this, "Force start with UN-stable rpy?");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyStableCheckConfigItem *this_ = ci->getParent<RpyStableCheckConfigItem>();
                this_->forceStart = !this_->forceStart;
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Check again.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyStableCheckConfigItem *this_ = ci->getParent<RpyStableCheckConfigItem>();
                this_->check(cc);
            };
        }
    }

    void check(ConfigContext &cc) {
        cc.logger->debug("checking rpy stable.");
        Result res;
        int stable = rpy->checkIfStable(res);
        this->stable = stable > 0;
    }

    bool isValid() override {
        return this->stable || this->forceStart;
    }

    void enter(ConfigContext &cc) override {
        if (!this->stable) {
            this->check(cc);
        }
        if (!this->stable) {
            // todo warn
        }
    }
};

class RpyBalanceCheckConfigItem : public ConfigItem {
    Rpy *rpy;
    bool balance = false;
    bool forceStart = false;

public:
    RpyBalanceCheckConfigItem(Rpy *rpy) {
        this->rpy = rpy;
    }
    String getTitle(String name) override {
        return String() << name << "(balance:" << balance << "forceStart:" << forceStart << ")";
    }
    bool isValid() {
        return this->balance || forceStart;
    }
    void onAttached() override {
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Force start with UN-balanced rpy?");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyBalanceCheckConfigItem *this_ = ci->getParent<RpyBalanceCheckConfigItem>();
                this_->forceStart = !this_->forceStart;
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Adjust the attitude of the quad copter and check again.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyBalanceCheckConfigItem *this_ = ci->getParent<RpyBalanceCheckConfigItem>();
                this_->check(cc);
            };
        }
    }
    void check(ConfigContext &cc) {
        Result res;
        balance = this->rpy->checkIfBalance(res) > 0;
    }
    void enter(ConfigContext &cc) override {
        this->check(cc);
        if (!this->balance) {
            // todo warn.
        }
    }
};
class PowerConfigItem : public ConfigItem {
    PowerManage *pm;

public:
    PowerConfigItem(PowerManage *pm) {
        this->pm = pm;
    }

    bool isValid() override {
        Result res;
        int ret = pm->isReady(res);
        return ret > 0;
    }

    String getTitle(String name) override {
        return String() << name << "(voltage:" << pm->getVoltage() << ")";
    }

    void enter(ConfigContext &cc) override {
        ConfigItem::enter(cc);
    }
};

class ExitConfigItem : public ConfigItem {
    bool &start;

public:
    ExitConfigItem(bool &start) : start(start) {
    }
    void onAttached() override {
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Toggle start after config?");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                ExitConfigItem *this_ = ci->getParent<ExitConfigItem>();
                this_->start = !this_->start;
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Start now.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                ExitConfigItem *this_ = ci->getParent<ExitConfigItem>();
                this_->start = true;
                cc.navigator->stop(1);
            };
        }
        }
    String getTitle(String name) override {
        return String() << name << "(start:" << start << ")";
    }
    void enter(ConfigContext &cc) override {
    }
};

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
        ConfigItems::addReturn(ci, String() << "", new PowerConfigItem(pm));
        logger->debug(".");
        ci = this;
        ci = ConfigItems::addReturn(ci, String() << "Rpy-check");
        logger->debug("..");
        {
            ConfigItems::add(ci, String() << "Rpy-stable-check", new RpyStableCheckConfigItem(rpy));
            logger->debug("...");
            ConfigItems::add(ci, String() << "Rpy-balance-check", new RpyBalanceCheckConfigItem(rpy));
        }
        logger->debug("....");
        ci = this;
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
        cc.navigator->enter(this->exitConfigItem->getDirectory());
    }

    void enter(ConfigContext &cc) override {

        cc.logger->debug(">>Config::config.");
        DirectoryNavigator<ConfigContext &, ConfigItem *> nav(cc.reader, cc.out, this->tree);
        cc.navigator = &nav;
        nav.onDirectoryEnter = [](ConfigContext &cc, DirectoryNavigator<ConfigContext &, ConfigItem *> *nav) {
            cc.logger->debug("dir enter.");
            Directory<ConfigItem *> *dir = nav->get();
            if (dir->isRoot()) {
                // ignore root,avoid recursive config.
                return;
            }
            ConfigItem *ci = dir->getElement();
            ci->enter(cc);
        };
        nav.onLeftFailure = [](ConfigContext &cc, DirectoryNavigator<ConfigContext &, ConfigItem *> *nav) {
            Directory<ConfigItem *> *dir = nav->get();
            ConfigItem *ci = dir->getElement();
            ci->onLeftFailure(cc);
        };

        cc.logger->debug(String() << "run...");

        int ret = nav.run(cc); // blocked here until stop nav.

        cc.logger->debug(String() << "<<Config::config,ret:" << ret);
    }
};
} // namespace a8::fc