#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
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
    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("start", start);
    }
    void enter(ConfigContext &cc) override {
    }
};

} // namespace a8::fc