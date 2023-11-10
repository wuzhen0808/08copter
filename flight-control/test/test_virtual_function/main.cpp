#include "a8/hal.h"
#include "a8/hal/System.h"
#include "a8/util/schedule/Thread.h"
#include "a8/util/schedule/Timer.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "a8/fc/Callback.h"
#include "a8/util/Runnable.h"
#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/native/NativeFreeRtosInitializer.h"
#include "a8/native/NativeCopter.h"
#include "TestSystem.h"
#include <gtest/gtest.h>
#include <iostream>

namespace a8::fc {
using namespace a8::util;
using namespace a8::hal;
using namespace std;
using a8::fc::Timer;
using a8::fc::Thread;
using a8::fc::Callback;
using a8::fc::Runnable;

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
} // namespace a8::fc
namespace a8::native {

class Dog : public a8::fc::Animal {
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
    a8::fc::Scheduler * scheduler = new a8::hal::freertos::FreeRtosScheduler();
    a8::fc::Copter *copter = new a8::native::NativeCopter(scheduler);
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