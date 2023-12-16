#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
class StartConfigItem : public ConfigItem {
    bool &startEnabled;

public:
    StartConfigItem(bool &start) : startEnabled(start) {
    }
    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("start", startEnabled);
    }
    void enter(ConfigContext &cc) override {
        cc.navigator->stop(1);
    }
};

} // namespace a8::fc