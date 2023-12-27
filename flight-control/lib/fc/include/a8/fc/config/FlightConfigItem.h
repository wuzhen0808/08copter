#pragma once
#include "a8/fc/PowerManage.h"
#include "a8/fc/Rpy.h"
#include "a8/fc/RpyMonitor.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/PropellersConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;
enum UnBalanceAction {
    ASK = 0,
    IGNORE = 1,
    IGNORE_IF_SAFE = 2,
    END_OF_MISSION = 3
};
enum BalanceMode {
    FULL = 0,
    ROLL = 1,
    PITCH = 2
};

/**
 * Test report:
 * pwmElevation,P,      I,  D,  pidLimit(actual),  pidLimitI(actual), actualMaxPwm
 * 200,         6.0,    0,  0,  150(-158),          75(0),
 *
 */
class FlightConfigItem : public ConfigItem {

    Reader *reader;
    Output *out;
    Scheduler *sch;
    RpyMonitor *rpyMonitor;
    PowerManage *pm;
    Rpy *rpy;

public:
    long tickTimeMs = 6;

    float elevationThrottle = 0.0f;
    float activeThrottle0 = 55;
    float activeThrottle1 = 55;
    float activeThrottle2 = 45;
    float activeThrottle3 = 45;

    long flyingTimeLimitSec = 15;
    bool enablePropeller = true;
    int stableCheckRetries = 5;
    int unBalanceAction = UnBalanceAction::IGNORE_IF_SAFE;
    float maxThrottle = 1000;
    // 18.75,0,4.5,volt@11.2
    // 19.5,0,3.5,volt@11.2 **

    double pidKp = 19.5; // volt@11.2
    double pidKi = 0.0;
    double pidKd = 3.5;
    double pidOutputLimit = 400.0; //
    double pidOutputLimitI = 200;
    bool enableStart = true;
    int maxRpyUpdateRetries = 5;
    int balanceMode = BalanceMode::FULL;

public:
    FlightConfigItem(Reader *reader, Output *out, PowerManage *pm, Rpy *rpy, Scheduler *sch) {
        this->reader = reader;
        this->out = out;
        this->sch = sch;
        this->rpy = rpy;
        this->pm = pm;
        this->rpyMonitor = new RpyMonitor(rpy, out, sch);
    }
    ~FlightConfigItem() {
        delete this->rpyMonitor;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<bool>("enablePropeller", this->enablePropeller);
        title.set<float>("Kp", this->pidKp);
        title.set<float>("Ki", this->pidKi);
        title.set<float>("Kd", this->pidKd);
    }

    void onAttached() override {
        ConfigItem *ci = this;
        this->attach(new Directory<ConfigItem *>("Root", 0));

        ci = ConfigItems::addReturn(ci, String() << "Boot-Options");
        {
            ConfigItems::addSelectInput<FlightConfigItem *>(
                ci, "unBalanceAction", unBalanceAction, this, 4, [](FlightConfigItem *this_, int idx) {
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
            ConfigItems::addSelectInput<FlightConfigItem *>(
                ci, "balanceMode", balanceMode, this, 3, [](FlightConfigItem *this_, int idx) {
                    String option;
                    switch (idx) {
                    case 0:
                        option = "FULL";
                        break;
                    case 1:
                        option = "ROLL";
                        break;
                    case 2:
                        option = "PITCH";
                        break;
                    }
                    return option;
                });
            ConfigItems::add(ci, "stableCheckRetries", stableCheckRetries);
            ConfigItems::add(ci, "enablePropeller", enablePropeller);
            ConfigItems::add(ci, "enableStart", this->enableStart);
            ConfigItems::add(ci, "tickTimeMs", tickTimeMs);
            ConfigItems::add(ci, "flyingTimeLimit(sec)", flyingTimeLimitSec);
            ConfigItems::add(ci, "maxThrottle", maxThrottle);
            ci = this;
        }
        ci = ConfigItems::addReturn(ci, "Rpy:");
        {
            ConfigItems::add(ci, "Rpy-check", new RpyConfigItem(rpy));
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
            }
            ci = this;
        }

        ConfigItems::add(ci, "Power-manage", new PowerConfigItem(pm));
        ConfigItems::add(ci, "Propellers", new PropellersConfigItem(activeThrottle0, activeThrottle1, activeThrottle2, activeThrottle3));
        
        ci = ConfigItems::addReturn(ci, "Pid-arguments:");
        {

            ConfigItems::add(ci, "pidOutputLimit", pidOutputLimit);
            ConfigItems::add(ci, "pidOutputLimitI", pidOutputLimitI);
            ConfigItems::add(ci, "Kp", pidKp);
            ConfigItems::add(ci, "Ki", pidKi);
            ConfigItems::add(ci, "Kd", pidKd);
            ci = this;
        }
    }
        
};
} // namespace a8::fc