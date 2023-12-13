#include "MyCommon.h"
#include "a8/fc/esp.h"
#include "a8/fc/esp/example.h"
#include "a8/hal/esp.h"

namespace study {
using namespace a8::fc::esp;
using namespace a8::hal::esp;
void log(const char *msg) {
    Serial.print(msg);
}

void log(const char *msg, bool ln) {
    log(msg);
    if (ln) {
        Serial.println();
    }
}
void log(const int msg) {
    Serial.print(msg);
}
void log(const int msg, bool ln) {
    log(msg);
    if (ln) {
        Serial.println();
    }
}

void log(const long msg) {
    Serial.print(msg);
}
void log(const float msg) {
    Serial.print(msg);
}
void log(const float msg, bool ln) {
    log(msg);
    if (ln) {
        Serial.println();
    }
}

void log(int x, int y, int z, bool ln) {
    log("(", false);
    log(x, false);
    log(",", false);
    log(y, false);
    log(",", false);
    log(z, false);
    log(")", ln);
}
void log(const char *prompt, int x, int y, int z, bool ln) {
    log(prompt, false);
    log(x, y, z, ln);
}
void log(float x, float y, float z, bool ln) {
    log("(", false);
    log(x, false);
    log(",", false);
    log(y, false);
    log(",", false);
    log(z, false);
    log(")", ln);
}
void log(const char *prompt, float x, float y, float z, bool ln) {
    log(prompt, false);
    log(x, y, z, ln);
}

int setupWire() {
    bool ok = Wire.begin(2, 1);
    if (!ok) {
        study::log("failed to setup wire.", true);
    } else {
        study::log("success of setup wire.", true);
    }
    return ok ? 1 : -1;
}

int setupSerial() {
    Serial.begin(115200);
    while (Serial.available()) {
        Serial.read();
    }
    return 1;
}
MPU9250 *mpu = new MPU9250();
System *sys = new EspSystem();
Scheduler *sch = new EspScheduler();
LoggerFactory *logFac = new EspLoggerFactory(sys, sch);
EspRpy *rpy = new EspRpy(mpu, logFac);
EspApplication *exa = new EspApplication(mpu);
int setupExample() {
    Result res;
    int ret = exa->start(res);
    if (ret < 0) {
        Serial.println("failed to setup example.");
    }
    return ret;
}
int setupMpu9250() {
    mpu->setup(0x68);
    mpu->selectFilter(QuatFilterSel::MADGWICK);
    mpu->setFilterIterations(1);
    study::log("success of setupMpu9250.", true);
    return 1;
}

// bool mpuUpdate() {
//     return exa->update();
// }
// float mpuGetRoll() {
//     return exa->getRoll();
// }
// float mpuGetPitch() {
//     return exa->getPitch();
// }
// float mpuGetYaw() {
//     return exa->getYaw();
// }

} // namespace study