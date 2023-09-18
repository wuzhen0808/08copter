#include "a8/hal/Hal.h"
#include "a8/hal/System.h"
#include "a8/core/Thread.h"
#include "a8/core/Timer.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/core/Callback.h"
#include "a8/core/Runnable.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/native/NativeFreeRtosInitializer.h"
#include "a8/native/NativeCopter.h"
#include "TestSystem.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using namespace std;
using a8::core::Timer;
using a8::core::Thread;
using a8::core::Callback;
using a8::core::Runnable;
namespace a8::core {

class Animal {
public:
    Animal(){};
    ~Animal(){};
    virtual int speak() = 0;
    virtual Timer * tmpTimer() = 0;
    virtual void startSchedule() = 0;
    virtual void endSchedule() = 0;
    virtual Thread *schedule(Runnable *runnable) = 0;

    virtual Timer *scheduleTimer(Callback *callback, long ticks) = 0;

    virtual Thread *getCurrentThread() = 0;
};
} // namespace a8::core
namespace a8::native {

class Dog : public a8::core::Animal {
public:
    Dog() {
    }
    ~Dog() {}
    virtual int speak() override {
        return 2;
    }
    virtual Timer* tmpTimer() override {
        return 0;
    }
    virtual void startSchedule() override{}

    virtual void endSchedule() override{}

    virtual Thread *schedule(Runnable *runnable) override{
        return 0;
    }

    virtual Timer *scheduleTimer(Callback *callback, long ticks) override{
        return 0;
    }

    virtual Thread *getCurrentThread() override{
        return 0;
    }
};
} // namespace a8::native

TEST(TestScheduler, virtualTest) {
    a8::native::FreeRtosInitializer *rtos = new a8::native::NativeFreeRtosInitializer();
    rtos->initialize();
    a8::core::Scheduler * scheduler = new a8::freertos::FreeRtosScheduler();
    a8::core::Copter *copter = new a8::native::NativeCopter(scheduler);
    copter->setup();//here????
    scheduler -> tmpTimer();
    cout << "DONE"<<endl;
}
/*
TEST(TestString, smockTest) {

    a8::native::Dog d1;

    // cout << (o1 + o2).value << endl;
    cout << d1.speak() << endl;
    EXPECT_EQ(2, d1.speak());
}
*/
System *a8::hal::S = new TestSystem();
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (RUN_ALL_TESTS())
        ;

    return 0;
}