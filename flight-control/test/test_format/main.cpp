#include "a8/util.h"
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>

using namespace a8::util;
using ::testing::EmptyTestEventListener;
using ::testing::InitGoogleTest;
using ::testing::Test;
using ::testing::TestEventListener;
using ::testing::TestEventListeners;
using ::testing::TestInfo;
using ::testing::TestPartResult;
using ::testing::UnitTest;
TEST(TestMath, testLog10) {
    float exp = Math::log10<float>(1.0f);
    EXPECT_EQ((int)exp, 0);
}

TEST(TestFormat, testFormatInt) {

    int len = 10;
    char buf[len];
    int len2 = Format::format<int>(buf, len, 1, false, true);
    // actual len should be plus 1 for the tail of string.
    EXPECT_EQ(len2, 1);
    EXPECT_EQ('1', buf[0]);
    EXPECT_EQ('\0', buf[1]);
}

TEST(TestFormat, testFormatNegInt) {
    int len = 10;
    char buf[len];
    int value = -1;
    int len2 = Format::format<int>(buf, len, value, false, true);
    EXPECT_EQ(len2, 2);
    EXPECT_EQ('-', buf[0]);
    EXPECT_EQ('1', buf[1]);
    EXPECT_EQ('\0', buf[2]);
}

TEST(TestFormat, testFormatFloat) {
    int len = 10;
    char buf[len];
    float value = 1.0f;
    int len2 = Format::format<float>(buf, len, value, 3, true);
    EXPECT_EQ(len2, 7);
    EXPECT_EQ('0', buf[0]);
    EXPECT_EQ('.', buf[1]);
    EXPECT_EQ('1', buf[2]);
    EXPECT_EQ('0', buf[3]);
    EXPECT_EQ('0', buf[4]);
    EXPECT_EQ('e', buf[5]);
    EXPECT_EQ('1', buf[6]);
    EXPECT_EQ('\0', buf[7]);
}

TEST(TestFormat, testFormatNegFloat) {
    int len = 10;
    char buf[len];
    float value = -1.0f;
    int len2 = Format::format<float>(buf, len, value, 3, true);
    EXPECT_EQ(len2, 8);
    EXPECT_EQ('-', buf[0]);
    EXPECT_EQ('0', buf[1]);
    EXPECT_EQ('.', buf[2]);
    EXPECT_EQ('1', buf[3]);
    EXPECT_EQ('0', buf[4]);
    EXPECT_EQ('0', buf[5]);
    EXPECT_EQ('e', buf[6]);
    EXPECT_EQ('1', buf[7]);
    EXPECT_EQ('\0', buf[8]);
}

TEST(TestFormat, testAppend) {
    int cap = 0;
    char *buf = 0;
    int len = 0;
    int value = 1;
    int fieldWidth = 3;
    Format::append<int>(buf, len, cap, 16, fieldWidth, ' ', value, 0, true);
    EXPECT_EQ(cap, 16);
    EXPECT_EQ(len, fieldWidth);
    EXPECT_EQ(' ', buf[0]);
    EXPECT_EQ(' ', buf[1]);
    EXPECT_EQ('1', buf[2]);
    EXPECT_EQ('\0', buf[3]);
}

TEST(TestFormat, testAppendNoFill) {
    int cap = 0;
    char *buf = 0;
    int len = 0;
    int value = 1;
    int fwidth = 0;
    Format::append<int>(buf, len, cap, 16, fwidth, ' ', value, 0, true);
    EXPECT_EQ(cap, 16);
    EXPECT_EQ(len, 1);
    EXPECT_EQ('1', buf[0]);
    EXPECT_EQ('\0', buf[1]);
}

TEST(TestFormat, testMore) {
    {

        String s;
        s << int(1);
        EXPECT_EQ(s, String("1"));
    }
    {

        String s;
        s << int(-1);
        EXPECT_EQ(s, String("-1"));
    }
    {

        String s;
        s << int(123);
        EXPECT_EQ(s, String("123"));
    }
    {

        String s;
        s << int(-321);
        EXPECT_EQ(s, String("-321"));
    }
    {

        String s;
        s << 1.0f;
        EXPECT_EQ(s, String("0.100e1"));
    }
    {

        String s;
        s << -1.0f;
        EXPECT_EQ(s, String("-0.100e1"));
    }
    {

        String s;
        s << 0.1f;
        EXPECT_EQ(s, String("0.100e0"));
    }
    {

        String s;
        s << -0.1f;
        EXPECT_EQ(s, String("-0.100e0"));
    }
    {

        String s;
        s << 12.3f;
        EXPECT_EQ(s, String("0.123e2"));
    }
    {

        String s;
        s << 0.00123f;
        EXPECT_EQ(s, String("0.123e-2"));
    }
    {

        String s;
        s << -0.00123f;
        EXPECT_EQ(s, String("-0.123e-2"));
    }

    {

        String s;
        s << 12.345678f;
        EXPECT_EQ(s, String("0.123e2"));
    }
}

int main(int argc, char **argv) {
    InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    // logFile.close();
    return 0;
}