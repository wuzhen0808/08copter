#include "TersePrinter.h"
#include "TestSystem.h"
#include "a8/hal/Hal.h"
#include "a8/native/NativeSystem.h"
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

TEST(TestString, testEndWith) {
    String msg = "ABCDEF";
    EXPECT_TRUE(msg.endWith("F"));
    EXPECT_TRUE(msg.endWith("EF"));
    EXPECT_TRUE(msg.endWith("DEF"));
    EXPECT_TRUE(msg.endWith("ABCDEF"));
    EXPECT_FALSE(msg.endWith("A"));
}
TEST(TestString, testAppend) {
    String msg;
    msg.append("ABC");
    msg.append("DEF");
    EXPECT_EQ("ABCDEF", msg);
}
TEST(TestString, testAssign) {
    String msg = "abc";
    EXPECT_EQ(msg, "abc");
    // msg = String("DEF");
    // EXPECT_EQ(msg, "DEF");
}

TEST(TestString, testFormat) {
    String msg = "[0]";
    EXPECT_EQ(3, msg.getLength());

    String msg2 = String::format("msg:%s", msg.getText());
    cout << msg2.getText() << endl;
    EXPECT_EQ(7, msg2.getLength());
    EXPECT_EQ(msg2, "msg:[0]");

    char buf[] = "set fcs/throttle-cmd-norm[0] 1\n";
    String msg3 = String::format("sending... message:%s", buf);
    EXPECT_EQ(msg3, "sending... message:set fcs/throttle-cmd-norm[0] 1\n");
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

TEST(TestString, formatIntTest) {
    String s31 = String::format("A%d", 8);
    EXPECT_EQ(s31, "A8");

    String s32 = String::format("A%d", 123);
    EXPECT_EQ(s32, "A123");

    cout << INT32_MAX << endl; // 2147483647
    cout << INT32_MIN << endl; //-2147483648

    String s33 = String::format("A%d", 1234567890);
    EXPECT_EQ(s33, "A1234567890");

    String s34 = String::format("A%d", INT32_MAX);
    EXPECT_EQ(s34, "A2147483647");

    String s35 = String::format("A%d", INT32_MIN);
    EXPECT_EQ(s35, "A-2147483648");

    EXPECT_TRUE(true);
}

void log(String msg) {
    // a8::hal::println(msg.getText());
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