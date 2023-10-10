#include "a8/hal.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Buffer.h"
#include "a8/util/String.h"
#include "test/TersePrinter.h"
#include "test/TestSystem.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using namespace a8::hal;
using namespace testing;

System *a8::hal::S = new test::TestSystem();

TEST(TestString, testOperatorAppend) {
    String msg2;
    msg2 << "["
         << "]";
    EXPECT_EQ(msg2, "[]");
}
TEST(TestString, testFormat) {
    long long time = S->getSysTime();
    String timeFormated;
    S->formatTime(time, &timeFormated);
    EXPECT_EQ(timeFormated.size(), sizeof("2023-10-10T19:27:00.999+0800"));    
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    UnitTest &unit_test = *UnitTest::GetInstance();
    TestEventListeners &listeners = unit_test.listeners();
    TestEventListener *defaultListener = listeners.default_result_printer();
    listeners.Append(new test::TersePrinter());
    int ret = RUN_ALL_TESTS();
    return 0;
}