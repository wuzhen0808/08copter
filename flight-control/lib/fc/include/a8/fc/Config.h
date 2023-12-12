#pragma once
namespace a8::fc {
class Config {
public:
    long tickTimeMs = 10;
    long pwmElevation = 200; // 210
    long flyingTimeLimitSec = 10;
    long delayBeforeStartSec = 3;
    bool enablePropeller = true;
    long pwmMax = 2000;
    long pwmMin = 1000;
    double pidKp = 6.0;
    double pidKi = 0.0;
    double pidKd = 0.0;
    float maxBalancePidOutput = 118;//(-300,300)
    float maxBalancePidIntegralOutput = 75;
};
} // namespace a8::fc