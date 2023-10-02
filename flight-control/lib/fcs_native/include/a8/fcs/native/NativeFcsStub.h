#pragma once
#include "a8/net/FcsStub.h"
using namespace a8::net;

namespace a8::hal::native {
// TODO this class should not extends the code from net, but only depends on the hal folder.
// Extract part of code to the FcsStub and other part should be some interface located in the hal folder.
class NativeFcsStub : public FcsStub {
    
};
} // namespace a8::hal::native