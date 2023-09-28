#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/hal/Hal.h"
#include "a8/hal/native/NativeSystem.h"
#include "a8/util/Buffer.h"
#include "a8/util/LineReader.h"
#include "a8/util/String.h"
#include "a8/util/StringReader.h"
#include "input_output/net_fdm.hxx"
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

TEST(TestLineReader, someTest1) {

    Reader *sr = new StringReader("A\nBC\nDEF\n");

    LineReader *lr = new LineReader(sr);
    String line;
    int rst1 = lr->readLine(line, true);
    EXPECT_EQ(rst1, 2);
    EXPECT_EQ(line, "A\n");

    int rst2 = lr->readLine(line, true);
    EXPECT_EQ(rst2, 3);
    EXPECT_EQ(line, "BC\n");

    int rst3 = lr->readLine(line, true);
    EXPECT_EQ(rst3, 4);
    EXPECT_EQ(line, "DEF\n");
}

TEST(TestLineReader, someTest2) {

    Reader *sr = new StringReader("A\nBC\nDEF\n");

    LineReader *lr = new LineReader(sr);
    String line;
    int rst1 = lr->readLine(line, false);
    EXPECT_EQ(rst1, 1 + 1);
    EXPECT_EQ(line, "A");

    int rst2 = lr->readLine(line, false);
    EXPECT_EQ(rst2, 2 + 1);
    EXPECT_EQ(line, "BC");

    int rst3 = lr->readLine(line, false);
    EXPECT_EQ(rst3, 3 + 1);
    EXPECT_EQ(line, "DEF");
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    UnitTest &unit_test = *UnitTest::GetInstance();
    TestEventListeners &listeners = unit_test.listeners();
    TestEventListener *defaultListener = listeners.default_result_printer();

    // logFile <<"The default event listener type is:" << typeid(defaultListener).name() << endl;
    // delete listeners.Release(defaultListener);
    listeners.Append(new TersePrinter());
    int ret = RUN_ALL_TESTS();
    // logFile.close();
    return 0;
}