#include "a8/jsbsim/JsbsimServo.h"
#include <FGFDMExec.h>
namespace a8::jsbsim {

void JsbsimServo::writeMicroseconds(int pw) {
}
void JsbsimServo::attach(int pin) {
    JSBSim::FGFDMExec exec;
    //exec.LoadScript(SGPath(argv[1]));
    exec.RunIC();
    bool result = true;
    while (result) result = exec.Run();
}

} // namespace a8::jsbsim