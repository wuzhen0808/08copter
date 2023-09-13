#ifndef A8_TIMER__
#define A8_TIMER__
#include "a8/core/core.h"
#include "a8/core/Callback.h"

namespace a8 {
namespace core {
class Timer {
public:
    Timer(){};
    ~Timer(){};
    virtual Timer *start() = 0;
};
} // namespace rtos
} // namespace a8

#endif