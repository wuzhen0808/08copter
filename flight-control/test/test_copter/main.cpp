#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/fcs/FlightControl.h"
#include "a8/hal/freertos/FreeRtosScheduler.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/NativeCopter.h"
#include "a8/hal/native/NativeFreeRtosInitializer.h"
#include "a8/hal/native/NativeSystem.h"

#include "a8/util/Buffer.h"
#include "a8/util/String.h"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
//
using ::a8::fcs::Copter;
using ::a8::hal::freertos::FreeRtosScheduler;
using ::a8::native::NativeFreeRtosInitializer;

System *a8::hal::S = new TestSystem();

using ::a8::hal::S;
void initialize() {

    S->out->println("Rtos initializing...");
    NativeFreeRtosInitializer *rtos = new NativeFreeRtosInitializer();
    rtos->initialize();
    S->out->println("Rtos ready.");
    Scheduler *scheduler = new FreeRtosScheduler();
    Copter *copter = new NativeCopter(scheduler);
    copter->setup();
    copter->start();
    cout << "Hello,main()";
}

TEST(TestCopter, smockTest1) {
}

int main(int argc, char **argv) {

    InitGoogleTest(&argc, argv);

    initialize();

    int ret = RUN_ALL_TESTS();
    return 0;
}