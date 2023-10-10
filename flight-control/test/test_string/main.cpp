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
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;

System *a8::hal::S = new test::TestSystem();
TEST(TestString, testFormat) {
    S->formatTime();
}

TEST(TestString, testEmpty) {
    String str0;
    int len0 = Lang::strLength(str0.text());
    EXPECT_EQ(0, len0);
}

TEST(TestString, testFormatLongString) {
    String str1 = "11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111";
    String str2 = String::format("abcdefghij0:%s", str1.text());
    String str3 = "abcdefghij0:11111111112222222222333333333344444444445555555555666666666677777777778888888888999999999900000000001111111111";

    EXPECT_EQ(str2.length(), str3.length());
    EXPECT_EQ(str2, str3);
}
TEST(TestString, testSplit) {
    String line = "A,BC,DEF,G";
    Buffer<String> fields = StringUtil::split(line, ',');
    EXPECT_EQ(4, fields.length());
    EXPECT_EQ(fields.get(0), "A");
    EXPECT_EQ(fields.get(1), "BC");
    EXPECT_EQ(fields.get(2), "DEF");
    EXPECT_EQ(fields.get(3), "G");

    int i0 = fields.indexOf("A");
    EXPECT_EQ(0, i0);
    int i1 = fields.indexOf("BC");
    EXPECT_EQ(1, i1);
    int i2 = fields.indexOf("DEF");
    EXPECT_EQ(2, i2);
    int i3 = fields.indexOf("G");
    EXPECT_EQ(3, i3);
}

TEST(TestString, testAppendOperator) {

    String line = "";
    int i = 1;
    line << i << "/";
    EXPECT_EQ(line, "1/");

    line = "";
    float f = 1.2f;
    line << f << "/";
    EXPECT_EQ(line, "1.200000e+00/");

    line = "";
    double d = 0.123456789D;
    line << d << "/";
    cout << line.text() << endl;
    EXPECT_EQ(line, "1.234568e-01/");

    line = "";
    String abc("abc");
    line << abc << "/";
    cout << line.text() << endl;
    EXPECT_EQ(line, "abc/");
}
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
    msg = String("DEF");
    EXPECT_EQ(msg, "DEF");
    msg = "GHI";
    EXPECT_EQ(msg, "GHI");
    msg = 1;
    EXPECT_EQ(msg, "1");
    msg = 1.1f;
    cout << msg.text() << endl;
    EXPECT_EQ(msg, "1.100000e+00");
}

TEST(TestString, testFormat) {
    String msg = "[0]";
    EXPECT_EQ(3, msg.length());

    String msg2 = String::format("msg:%s", msg.text());
    cout << msg2.text() << endl;
    EXPECT_EQ(7, msg2.length());
    EXPECT_EQ(msg2, "msg:[0]");

    char buf[] = "set fcs/throttle-cmd-norm[0] 1\n";
    String msg3 = String::format("sending... message:%s", buf);
    EXPECT_EQ(msg3, "sending... message:set fcs/throttle-cmd-norm[0] 1\n");
}

TEST(TestString, smockTest1) {

    String s1;
    EXPECT_EQ(s1.length(), 0);

    String s2 = s1;
    EXPECT_EQ(s2.length(), 0);

    EXPECT_TRUE(s1 == s2);
    String *s3 = new String();
    EXPECT_TRUE(*s3 == s2);
    delete s3;
}

TEST(TestString, smockTest2) {
    String s1 = "A";
    String s2 = "8";

    EXPECT_EQ(s1.length(), 1);
    EXPECT_EQ(s2.length(), 1);
    EXPECT_FALSE(s1 == s2);

    String s3 = s1 + s2;
    EXPECT_EQ(s3.length(), 2);
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
    // a8::hal::println(msg.text());
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    UnitTest &unit_test = *UnitTest::GetInstance();
    TestEventListeners &listeners = unit_test.listeners();
    TestEventListener *defaultListener = listeners.default_result_printer();

    // logFile <<"The default event listener type is:" << typeid(defaultListener).name() << endl;
    // delete listeners.Release(defaultListener);
    listeners.Append(new test::TersePrinter());
    int ret = RUN_ALL_TESTS();
    // logFile.close();
    return 0;
}