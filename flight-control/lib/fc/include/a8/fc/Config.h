#pragma once
namespace a8::fc {
class Config {
public:
    long pwmElevation = 100;//210
    long pwmLimitLH = -1; // 1000;
    long pwmLimitRH = -1; // 1000;
    long pwmLimitLA = -1;
    long pwmLimitRA = -1;
    long flyingTimeLimitSec = 10;
    double pidKp = 3.55;
    double pidKi = 0.3;
    double pidKd = 2.05;
};
} // namespace a8::fc