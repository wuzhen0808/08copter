#include <FGFDMExec.h>

int jsbsim_minimal_main(int argc, char **argv)
{
  JSBSim::FGFDMExec FDMExec;

  FDMExec.LoadScript(SGPath(argv[1]));

  while (FDMExec.Run());
  return 0;
}

