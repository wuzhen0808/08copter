#ifndef Callback__
#define Callback__
namespace a8 {
namespace rtos {

class Callback {
public:
    virtual void call() = 0;
};

} // namespace rtos
} // namespace a8
#endif