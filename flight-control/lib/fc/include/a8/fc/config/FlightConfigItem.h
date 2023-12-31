#pragma once
#include "a8/fc/GlobalVars.h"
#include "a8/fc/Imu.h"
#include "a8/fc/ImuMonitor.h"
#include "a8/fc/PowerManage.h"
#include "a8/fc/config/PowerConfigItem.h"
#include "a8/fc/config/PropellersConfigItem.h"
#include "a8/fc/config/RpyConfigItem.h"
#include "a8/fc/defines.h"
#include "a8/util.h"

namespace a8::fc {
using namespace a8::util;

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
    ImuMonitor *imuMonitor;
    PowerManage *pm;
    Imu *imu;

public:
    TimeUs tickTimeUs = 6000;
    // rpy
    ImuFilter imuFilter = ImuFilter::NONE;
    int imuFilterIterations = 1;

    float elevationThrottle = 0.0f;
    float activeThrottle0 = 55;
    float activeThrottle1 = 55;
    float activeThrottle2 = 45;
    float activeThrottle3 = 45;

    long flyingTimeLimitSec = 10;
    bool lockPropellers = true;
    int stableCheckRetries = 10;
    UnBalanceAction unBalanceAction = UnBalanceAction::IGNORE_IF_SAFE;
    float unBalanceDegUpLimit = 10.0f;
    float unBalanceDegLimit = 5.0f;
    float maxThrottle = 1000;

    //////////////////////
    // 20,6,4.25,v11.2,f(NO->LP1) --(This is only work at first flight, because LP filter does not address cumulative bias.)
    // 19.5,6.0,4.25,v11.4,f(MAG->NO) ***, 
    // 18.75,0,4.5,v11.2
    // 19.5,0,3.5,v11.2 **
    // 19.5,5,3.9,v12.2/11.8 *
    // 19.5,6.5,4.25,v11.0 *
    // 18,16.5,4.1,v11.3
    // 19.5,5,4.25,v11.6 : very fast flapping.
    // 19.5,5,4.1,v11.7 : mid fast flapping.
    // 19.5,6,4.1,v11.7 : mid shaving.

    //////////////////////
    // imuFilter:empty;
    // rpyFilter:LP1
    //////////////////////
    double pidKp = 19.5; // volt@11.2
    double pidKi = 5.0;
    double pidKd = 3.9;
    double pidOutputLimit = 500.0; //
    double pidOutputLimitI = 500.0;
    bool pidEnableErrorDiffFilter = true;
    float pidErrorDiffFilterCutOffRate = 50.0f;
    int pidErrorDiffFilterOrder = 2;

    double yawPidKp = 0;
    double yawPidKi = 0;
    double yawPidKd = 0;
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
    float minVoltForPwmCompensate = 9.0f;

    int maxImuUpdateRetries = 5;
    RpyFilter rpyFilter = RpyFilter::NO; // LP1 don't get the Pid balanceed.

    float rpyFilterCutOffHz = 20.0f;

    GlobalVars &vars;

public:
    FlightConfigItem(GlobalVars &vars, Reader *reader, Output *out, PowerManage *pm, Imu *imu, Scheduler *sch) : vars(vars) {
        this->reader = reader;
        this->out = out;
        this->sch = sch;
        this->imu = imu;
        this->pm = pm;
        this->imuMonitor = new ImuMonitor(imu, out, sch);
    }
    ~FlightConfigItem() {
        delete this->imuMonitor;
    }

    void buildTitle(ConfigItem::TitleBuilder &title) override {
        title.set<long>("flySec", this->flyingTimeLimitSec);
        title.set<bool>("lockProps", this->lockPropellers);
        title.set<float>("Kp", this->pidKp);
        title.set<float>("Ki", this->pidKi);
        title.set<float>("Kd", this->pidKd);
        title.set<bool>("enFg", this->enableForeground);
        title.set<long>("countDown", this->preStartCountDown);
    }

    void onAttached() override {
        ConfigItem *ci = this;
        ci = ConfigItems::addReturn(ci, String() << "Basic-Options");
        {
            ConfigItems::add(ci, "lockPropellers", this->lockPropellers);
            ConfigItems::add(ci, "preStartDelaySec", this->preStartCountDown);
            ConfigItems::add(ci, "enableForeground", this->enableForeground);
            ConfigItems::addSelectInput<UnBalanceAction>(ci, "unBalanceAction", unBalanceAction, &UN_BALANCE_ACTIONS);
            ConfigItems::add(ci, "stableCheckRetries", stableCheckRetries);
            ConfigItems::add(ci, "enableStart", this->enableStart);
            ConfigItems::add(ci, "tickTimeUs", tickTimeUs);
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
            {
                ConfigItems::addSelectInput<ImuFilter>(ci, "imuFilter", imuFilter, &IMU_FILTERS);
                ConfigItems::add(ci, "imuFilterIterations", imuFilterIterations);
            }
            {
                ci = ci->getLastChild();
                ci->setAttribute(0, this, Lang::empty<void *>);
                ci->onAfterEnter = [](ConfigContext &cc) {
                    FlightConfigItem *this_ = cc.getConfigItem()->getAttribute<FlightConfigItem *>(0, 0);
                    this_->imu->setFilter(this_->imuFilter, this_->imuFilterIterations);
                };
                ci = ci->getParent<ConfigItem>();
            }
            {
                ConfigItems::addSelectInput<RpyFilter>(ci, "rpyFilter", rpyFilter, &RPY_FILTERS);
            }
            ConfigItems::add(ci, "rpyFilterCutOffHz", rpyFilterCutOffHz);
            ConfigItems::add(ci, "Rpy-check", new RpyConfigItem(imu, this->unBalanceDegLimit));
            ConfigItems::add(ci, "maxImuUpdateRetries", maxImuUpdateRetries);
            ci = ConfigItems::addReturn(ci, "Monitor-rpy");
            {
                ci->setAttribute(imuMonitor, [](void *) {});
                ci->onBuildTitle = [](TitleBuilder &title) {
                    ImuMonitor *imuMonitor = title.configItem->getAttribute<ImuMonitor *>(0);
                    title.set<bool>("Running", imuMonitor->isRunning());
                };
                ci->onEnter = [](ConfigContext &cc) {
                    ImuMonitor *imuMonitor = cc.navigator->get()->getAttribute<ImuMonitor *>(0);
                    if (imuMonitor->isRunning()) {
                        imuMonitor->close();
                    } else {
                        imuMonitor->open();
                    }
                };
                ConfigItems::add<ImuMonitor *, float>(
                    ci, "Set-rate", imuMonitor,                                           //
                    [](ImuMonitor *imuMonitor) { return imuMonitor->getRate().Hz(); },    //
                    [](ImuMonitor *imuMonitor, float rate) { imuMonitor->setRate(rate); } //
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

        ConfigItems::add(ci, "Propellers", new PropellersConfigItem(activeThrottle0, activeThrottle1, activeThrottle2, activeThrottle3));
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