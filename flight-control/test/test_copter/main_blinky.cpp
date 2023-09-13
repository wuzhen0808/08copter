#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/core/Copter.h"
#include "a8/freertos/FreeRtosScheduler.h"
#include "a8/hal/Hal.h"
#include "a8/native/NativeSystem.h"
#include "a8/native/NativeCopter.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

System *a8::hal::S = new TestSystem();
using ::a8::freertos::FreeRtosScheduler;
using ::a8::core::Copter;
Scheduler * scheduler = new FreeRtosScheduler();
Copter *copter = new NativeCopter(scheduler);
TEST(TestCopter, smockTest1) {
    copter->setup();
    copter->start();
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    return 0;
}