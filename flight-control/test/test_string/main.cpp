#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/hal/Hal.hpp"
#include "a8/native/NativeSystem.h"
#include "a8/util/Buffer.hpp"
#include "a8/util/String.hpp"
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

// TEST(TestBuffer, smokeTest) {
//     //Buffer<char> *buffer = new Buffer<char>();
//     //EXPECT_EQ(buffer->getLength(), 0);

// }
void log(String msg) {
    // a8::hal::println(msg.getText());
}

TEST(TestString, smockTest1) {

    String s1;
    EXPECT_EQ(s1.getLength(), 0);

    String s2 = s1;
    EXPECT_EQ(s2.getLength(), 0);

    EXPECT_TRUE(s1 == s2);
    String *s3 = new String();
    EXPECT_TRUE(*s3 == s2);
    delete s3;
}

TEST(TestString, smockTest2) {
    String s1 = "A";
    String s2 = "8";

    EXPECT_EQ(s1.getLength(), 1);
    EXPECT_EQ(s2.getLength(), 1);
    EXPECT_FALSE(s1 == s2);

    String s3 = s1 + s2;
    EXPECT_EQ(s3.getLength(), 2);
    EXPECT_EQ(s3, "A8");
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