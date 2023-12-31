#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/fc/collect/NamedExpr.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;

class CollectorSetup {

public:
    static int setupFlight(FlightConfigItem *config, Collector *collector, Result &res) {
        Buffer<String> nameWithExprs;
        nameWithExprs.add("rowNum[rowNum()]");
        nameWithExprs.add("missionId");
        nameWithExprs.add("timeMs");
        nameWithExprs.add("tickCostTimeMs");
        nameWithExprs.add("tickCostTimeMsAvg[avg(tickCostTimeMs)]");
        nameWithExprs.add("voltage");
        nameWithExprs.add("maxPwmC[maxOf(prop0-pwmC,prop1-pwmC,prop2-pwmC,prop3-pwmC)]");
        nameWithExprs.add("maxPwmT[maxOf(prop0-pwmT,prop1-pwmT,prop2-pwmT,prop3-pwmT)]");

        nameWithExprs.add("rawRoll");
        nameWithExprs.add("rawPitch");
        nameWithExprs.add("rawYaw");
        nameWithExprs.add("roll");
        nameWithExprs.add("pitch");
        nameWithExprs.add("yaw");
        nameWithExprs.add("rollDiff[diff(roll)]");
        nameWithExprs.add("pitchDiff[diff(pitch)]");
        nameWithExprs.add("yawDiff[diff(yaw)]");
        nameWithExprs.add("maxPwm[maxOf(prop0-pwm,prop1-pwm,prop2-pwm,prop3-pwm)]");
        nameWithExprs.add("avgMaxPwm[avg(maxPwm)]");
        nameWithExprs.add("RollPid-err0");
        nameWithExprs.add("RollPid-err1");
        nameWithExprs.add("RollPid-err0Diff");
        nameWithExprs.add("RollPid-err1Diff");
        nameWithExprs.add("RollPid-ets");
        nameWithExprs.add("RollPid-etMs[*(RollPid-ets,1000)]");
        nameWithExprs.add("RollPid-p");
        nameWithExprs.add("RollPid-i");
        nameWithExprs.add("RollPid-d");
        nameWithExprs.add("RollPid-o");
        //
        nameWithExprs.add("PitchPid-err0");
        nameWithExprs.add("PitchPid-err1");
        nameWithExprs.add("PitchPid-err0Diff");
        nameWithExprs.add("PitchPid-err1Diff");
        nameWithExprs.add("PitchPid-ets");
        nameWithExprs.add("PitchPid-etMs[*(PitchPid-ets,1000)]");
        nameWithExprs.add("PitchPid-p");
        nameWithExprs.add("PitchPid-i");
        nameWithExprs.add("PitchPid-d");
        nameWithExprs.add("PitchPid-o");
        //
        nameWithExprs.add("YawPid-err0");
        nameWithExprs.add("YawPid-err1");
        nameWithExprs.add("YawPid-err0Diff");
        nameWithExprs.add("YawPid-err1Diff");        
        nameWithExprs.add("YawPid-etMs[*(YawPid-ets,1000)]");
        nameWithExprs.add("YawPid-p");
        nameWithExprs.add("YawPid-i");
        nameWithExprs.add("YawPid-d");
        nameWithExprs.add("YawPid-o");

        nameWithExprs.add("prop0-pwm");
        nameWithExprs.add("prop1-pwm");
        nameWithExprs.add("prop2-pwm");
        nameWithExprs.add("prop3-pwm");

        /*
        names.add("timeMs[diff]");
        names.add("roll");
        names.add("roll[diff]");
        names.add("pitch");
        names.add("pitch[diff]");
        names.add("yaw");
        names.add("yaw[diff]");
        names.add("RollPid-err");
        names.add("RollPid-err[diff]");
        names.add("RollPid-ets");
        names.add("RollPid-ets[*1000]");
        names.add("RollPid-lmt");
        names.add("RollPid-lmtI");
        names.add("RollPid-p");
        names.add("RollPid-i");
        names.add("RollPid-d");
        names.add("RollPid-output");
        names.add("PitchPid-ets");
        names.add("PitchPid-ets*1000");
        names.add("PitchPid-err");
        names.add("PitchPid-err[diff]");
        names.add("PitchPid-lmt");
        names.add("PitchPid-lmtI");
        names.add("PitchPid-p");
        names.add("PitchPid-i");
        names.add("PitchPid-d");
        names.add("PitchPid-output");
        names.add("prop0-pwm");
        names.add("prop1-pwm");
        names.add("prop2-pwm");
        names.add("prop3-pwm");
        */

        Buffer<NamedExpr> nameExprs;
        int ret = NamedExpr::parseAll(nameWithExprs, &nameExprs, res);
        if (ret > 0) {
            ret = collector->addAllIfNotExists(nameExprs, res);
        }

        if (ret > 0) {
            collector->setDefaultEnable(false);
            collector->enable(nameExprs.transform<String>([](NamedExpr nExpr) { return nExpr.name; }), true);
        }
        if (ret > 0)
            ret = collector->setup(res);
        return ret;
    }
};
} // namespace a8::fc