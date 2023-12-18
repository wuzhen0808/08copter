#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
class RpyConfigItem : public ConfigItem {
    Rpy *rpy;
    bool enableStartWithUnStable = false;
    bool enableStartWithUnBalance = true;

public:
    RpyConfigItem(Rpy *rpy) {
        this->rpy = rpy;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("stable", rpy->isStable());
        title.set<bool>("balance", rpy->isBalance());
        title.set<bool>("forceStart", enableStartWithUnStable);
    }

    void onAttached() override {
        {

            ConfigItem *ci = ConfigItems::addReturn(this, "Enable start with UN-stable rpy?");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                this_->enableStartWithUnStable = !this_->enableStartWithUnStable;
            };
            ci->onBuildTitle = [](ConfigItem::TitleBuilder &title) {
                RpyConfigItem *rpyConfigItem = title.configItem->getParent<RpyConfigItem>();
                title.set<bool>("value", rpyConfigItem->enableStartWithUnStable);
            };
        }
        {

            ConfigItem *ci = ConfigItems::addReturn(this, "Enable start with UN-balance rpy?");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                this_->enableStartWithUnStable = !this_->enableStartWithUnStable;
            };
            ci->onBuildTitle = [](ConfigItem::TitleBuilder &title) {
                RpyConfigItem *rpyConfigItem = title.configItem->getParent<RpyConfigItem>();
                title.set<bool>("value", rpyConfigItem->enableStartWithUnStable);
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Check stable.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                Result res;
                this_->rpy->checkIfStable(res);
            };
        }
        {
            ConfigItem *ci = ConfigItems::addReturn(this, "Check balance.");
            ci->onEnter = [](ConfigContext &cc) {
                ConfigItem *ci = cc.navigator->get()->getElement();
                RpyConfigItem *this_ = ci->getParent<RpyConfigItem>();
                Result res;
                this_->rpy->checkIfBalance(res);
            };
        }
    }

    bool isValid() override {
        return (this->enableStartWithUnStable || this->rpy->isStable()) && (this->enableStartWithUnBalance || this->rpy->isBalance());
    }

    void enter(ConfigContext &cc) override {

        A8_LOG_DEBUG(cc.logger, "checking rpy stable.");
        Result res;
        int stable = rpy->checkIfStable(res);
        A8_LOG_DEBUG(cc.logger, "end of checking rpy stable.");
        this->rpy->checkIfBalance(res);
    }
};

} // namespace a8::fc