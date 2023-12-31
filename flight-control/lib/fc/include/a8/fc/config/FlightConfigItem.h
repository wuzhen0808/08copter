#pragma once
#include "a8/fc/GlobalVars.h"
#include "a8/fc/Imu.h"
#include "a8/fc/PowerManage.h"
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
    PITCH = 2,
    YAW = 3,
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
    bool lockPropellers = true;
    int stableCheckRetries = 10;
    int unBalanceAction = UnBalanceAction::IGNORE_IF_SAFE;
    float unBalanceDegUpLimit = 10.0f;
    float unBalanceDegLimit = 5.0f;
    float maxThrottle = 1000;
    // 18.75,0,4.5,volt@11.2
    // 19.5,0,3.5,volt@11.2 **
    // 19.5,5,3.9,volt@12.2 *
    // 19.5,6.5,4.25,volt@11.0 *
    // 18,16.5,4.1,volt@11.3

    double pidKp = 19.5; // volt@11.2
    double pidKi = 5.0;
    double pidKd = 3.9;
    double pidOutputLimit = 500.0; //
    double pidOutputLimitI = 500.0;
    bool pidEnableErrorDiffFilter = true;
    float pidErrorDiffFilterCutOffRate = 15.0f;
    int pidErrorDiffFilterOrder = 2;

    double yawPidKp = 9.5;
    double yawPidKi = 3.5;
    double yawPidKd = 2.25;
    double yawPidOutputLimit = 150.0; //
    double yawPidOutputLimitI = 150.0;
    int yawPidErrorDiffMAWidth = 1;

    bool enableStart = true;
    int balanceMode = BalanceMode::FULL;
    int collectEveryTicks = 1;
    bool ignorePowerWarning = false;

    long preStartCountDown = 10;
    bool enableForeground = false;

    bool enableVoltageCompensate = true;
    float maxVoltForPwmCompensate = 12.5f;
    float minVoltForPwmCompensate = 7.0f;

    GlobalVars &vars;

public:
    FlightConfigItem(GlobalVars &vars, Reader *reader, Output *out, PowerManage *pm, Rpy *rpy, Scheduler *sch) : vars(vars) {
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
        title.set<bool>("lockPropellers", this->lockPropellers);
        title.set<float>("Kp", this->pidKp);
        title.set<float>("Ki", this->pidKi);
        title.set<float>("Kd", this->pidKd);
        title.set<bool>("enableForeground", this->enableForeground);
        title.set<long>("preStartDelaySec", this->preStartCountDown);
    }

    void onAttached() override {
        ConfigItem *ci = this;
        ci = ConfigItems::addReturn(ci, String() << "Basic-Options");
        {
            ConfigItems::add(ci, "preStartDelaySec", this->preStartCountDown);
            ConfigItems::add(ci, "enableForeground", this->enableForeground);
            Buffer<String> options;
            options.add("ASK");
            options.add("IGNORE");
            options.add("IGNORE_IF_SAFE");
            options.add("END_MISSION");
            ConfigItems::addSelectInput(ci, "unBalanceAction", unBalanceAction, options);
            ConfigItems::add(ci, "stableCheckRetries", stableCheckRetries);
            ConfigItems::add(ci, "enableStart", this->enableStart);
            ConfigItems::add(ci, "tickTimeMs", tickTimeMs);
            ConfigItems::add(ci, "flyingTimeLimit(sec)", flyingTimeLimitSec);
            ConfigItems::add(ci, "unBalanceDegDownLimit", unBalanceDegLimit);
            ConfigItems::add(ci, "unBalanceDegUpLimit", unBalanceDegUpLimit);

            ci = this;
        }

        ci = ConfigItems::addReturn(ci, "Limits");
        {
            ConfigItems::add(ci, "maxThrottle", maxThrottle);
            ci = this;
        }

        ci = ConfigItems::addReturn(ci, "Collector");
        {
            ConfigItems::add(ci, "collectEveryTicks", collectEveryTicks);
            ci = this;
        }
        ci = ConfigItems::addReturn(ci, "Rpy");
        {
            ConfigItems::add(ci, "Rpy-check", new RpyConfigItem(rpy, this->unBalanceDegLimit));
            ConfigItems::add(ci, "rpyMovingAvgWindowWidth", vars.rpyMovingAvgWindowWidth);
            ConfigItems::add(ci, "maxRpyUpdateRetries", vars.maxRpyUpdateRetries);
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

        ci = ConfigItems::addReturn(ci, "Power-manage", new PowerConfigItem(pm, this->ignorePowerWarning));
        {

            ConfigItems::add(ci, "maxVoltForPwmCompensate", maxVoltForPwmCompensate);
            ConfigItems::add(ci, "minVoltForPwmCompensate", minVoltForPwmCompensate);
            ci = this;
        }

        ConfigItems::add(ci, "Propellers", new PropellersConfigItem(lockPropellers, activeThrottle0, activeThrottle1, activeThrottle2, activeThrottle3));
        ci = ConfigItems::addReturn(ci, "Pid-arguments");
        {

            ci = ConfigItems::addReturn(ci, "Roll/Pitch-pid");
            {

                ConfigItems::add(ci, "pidOutputLimit", pidOutputLimit);
                ConfigItems::add(ci, "pidOutputLimitI", pidOutputLimitI);
                ConfigItems::add(ci, "Kp", pidKp);
                ConfigItems::add(ci, "Ki", pidKi);
                ConfigItems::add(ci, "Kd", pidKd);

                ci = ci->getParent<ConfigItem>();
            } // end of pid.
            ci = ConfigItems::addReturn(ci, "Yaw-pid");
            {

                ConfigItems::add(ci, "pidOutputLimit", yawPidOutputLimit);
                ConfigItems::add(ci, "pidOutputLimitI", yawPidOutputLimitI);
                ConfigItems::add(ci, "pidErrorDiffMAWidth", yawPidErrorDiffMAWidth);
                ConfigItems::add(ci, "Kp", yawPidKp);
                ConfigItems::add(ci, "Ki", yawPidKi);
                ConfigItems::add(ci, "Kd", yawPidKd);

                ci = ci->getParent<ConfigItem>();
            } // end of pid.
            
            ConfigItems::add(ci, "pidEnableErrorDiffFilter", pidEnableErrorDiffFilter);
            ConfigItems::add(ci, "pidErrorDiffFilterCutOffRate", pidErrorDiffFilterCutOffRate);
            ConfigItems::add(ci, "pidErrorDiffFilterOrder", pidErrorDiffFilterOrder);
            

            ci = ci->getParent<ConfigItem>();
        }

        ci = ConfigItems::addReturn(ci, "Test-options");
        {
            Buffer<String> options;
            options.add("FULL");
            options.add("ROLL");
            options.add("PITCH");
            options.add("YAW");
            ConfigItems::addSelectInput(ci, "balanceMode", balanceMode, options); // end of balanceMode.
            ci = this;
        }
    }
};
} // namespace a8::fc