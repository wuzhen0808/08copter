#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class RpyConfigItem : public ConfigItem {
    Rpy *rpy;
    bool forceStart = false;
    bool stable = false;
    bool balance = false;

public:
    RpyConfigItem(Rpy *rpy) {
        this->rpy = rpy;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("stable", stable);
        title.set<bool>("balance", balance);
        title.set<bool>("forceStart", forceStart);
    }

    void onAttached() override {
        {

            ConfigItem *ci = ConfigItems::addReturn(this, "Force start with UN-stable or UN-balance rpy?");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                this_->forceStart = !this_->forceStart;
            };
            ci->onBuildTitle = [](ConfigItem::TitleBuilder &title) {
                RpyConfigItem *rpyConfigItem = title.configItem->getParent<RpyConfigItem>();
                title.set<bool>("value", rpyConfigItem->forceStart);
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Check stable again.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                this_->checkStable(cc);
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Check balance again.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                this_->checkBalance(cc);
            };
        }
    }

    void checkStable(ConfigContext &cc) {
        cc.logger->debug("checking rpy stable.");
        Result res;
        int stable = rpy->checkIfStable(res);
        this->stable = stable > 0;
    }

    void checkBalance(ConfigContext &cc) {
        Result res;
        balance = this->rpy->checkIfBalance(res) > 0;
    }

    bool isValid() override {
        return this->stable && this->balance || this->forceStart;
    }

    void enter(ConfigContext &cc) override {
        if (!this->stable) {
            this->checkStable(cc);
        }
        if (!this->balance) {
            this->checkBalance(cc);
        }
        if (!this->stable) {
            // todo warn
        }
    }
};

} // namespace a8::fc