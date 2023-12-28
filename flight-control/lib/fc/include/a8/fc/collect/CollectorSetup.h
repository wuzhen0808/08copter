#pragma once
#include "a8/fc/collect/Collector.h"
#include "a8/fc/collect/NamedExpr.h"
#include "a8/util.h"
namespace a8::fc {
using namespace a8::util;
using namespace a8::fc::collect;

class CollectorSetup {

public:
    static int setupFlight(Collector *collector, Result &res) {
        Buffer<String> nameWithExprs;
        nameWithExprs.add("rowNum[rowNum()]");
        nameWithExprs.add("timeMs");
        nameWithExprs.add("tickCostTimeMs");
        nameWithExprs.add("tickCostTimeMsAvg[avg(tickCostTimeMs)]");
        nameWithExprs.add("roll");
        nameWithExprs.add("pitch");
        nameWithExprs.add("yaw");
        nameWithExprs.add("rollDiff[diff(roll)]");
        nameWithExprs.add("pitchDiff[diff(pitch)]");
        nameWithExprs.add("yawDiff[diff(yaw)]");
        nameWithExprs.add("maxPwm[maxOf(prop0-pwm,prop1-pwm,prop2-pwm,prop3-pwm)]");
        nameWithExprs.add("avgMaxPwm[avg(maxPwm)]");
        nameWithExprs.add("RollPid-err");
        nameWithExprs.add("RollPid-errDiff");
        nameWithExprs.add("RollPid-ets");
        nameWithExprs.add("RollPid-etMs[*(RollPid-ets,1000)]");
        nameWithExprs.add("RollPid-p");
        nameWithExprs.add("RollPid-i");
        nameWithExprs.add("RollPid-d");
        nameWithExprs.add("RollPid-o");
        //
        nameWithExprs.add("PitchPid-err");
        nameWithExprs.add("PitchPid-errDiff");
        nameWithExprs.add("PitchPid-ets");
        nameWithExprs.add("PitchPid-etMs[*(PitchPid-ets,1000)]");
        nameWithExprs.add("PitchPid-p");
        nameWithExprs.add("PitchPid-i");
        nameWithExprs.add("PitchPid-d");
        nameWithExprs.add("PitchPid-o");

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