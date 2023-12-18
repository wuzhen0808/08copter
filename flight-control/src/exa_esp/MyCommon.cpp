#include "MyCommon.h"
#include "a8/fc/esp.h"
#include "a8/fc/esp/example.h"
#include "a8/hal/esp.h"

namespace study {
using namespace a8::fc::esp;
using namespace a8::hal::esp;

// System *sys = new EspSystem();
// Scheduler *sch = new EspScheduler();
// LoggerFactory *logFac = new EspLoggerFactory(sys, sch);


// int setupMpu9250() {
//     mpu->setup(0x68);
//     mpu->selectFilter(QuatFilterSel::MADGWICK);
//     mpu->setFilterIterations(1);
//     study::log("success of setupMpu9250.", true);
//     return 1;
// }

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