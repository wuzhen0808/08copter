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
        // nameWithExprs.add("maxPwm[maxOf(prop0-pwm,prop1-pwm,prop2-pwm,prop3-pwm)]");
        // nameWithExprs.add("avgMaxPwm[avg(maxPwm)]");
        nameWithExprs.add("PidR-ets");
        nameWithExprs.add("PidR-etMs[*(PidR-ets,1000)]");
        nameWithExprs.add("PidR-p");
        nameWithExprs.add("PidR-i");
        nameWithExprs.add("PidR-d");
        nameWithExprs.add("PidR-o");

        nameWithExprs.add("PidP-ets");
        nameWithExprs.add("PidP-etMs[*(PidP-ets,1000)]");
        nameWithExprs.add("PidP-p");
        nameWithExprs.add("PidP-i");
        nameWithExprs.add("PidP-d");
        nameWithExprs.add("PidP-o");
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