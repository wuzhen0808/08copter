#pragma once
#include "a8/util.h"

namespace a8::util::schedule {
using namespace a8::util;
class Semaphore {
public:
    Semaphore(){}
    ~Semaphore(){}
    virtual int take(long timeout, Result &res) = 0;
    virtual int give(Result &res) = 0;    
};

} // namespace a8::util::schedule
